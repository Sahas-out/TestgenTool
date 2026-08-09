#include "teststringGenerator/genetic_generator.hh"

#include <algorithm>
#include <stdexcept>

namespace ga {

// How many times initialisation may ask for a fresh batch before giving up.
// Only relevant once a real validator can reject sequences; it exists so a
// validator that refuses everything fails loudly instead of looping forever.
static const int MAX_INITIALIZATION_ATTEMPTS = 20;

GeneticGenerator::GeneticGenerator(GaConfig                          config,
                                   unique_ptr<GaFactory>             factory,
                                   unique_ptr<PopulationInitializer> initializer,
                                   unique_ptr<TestStringValidator>   validator)
    : config(config),
      factory(std::move(factory)),
      initializer(std::move(initializer)),
      validator(std::move(validator)),
      rng(config.seed)
{
    if (!this->factory || !this->initializer || !this->validator) {
        throw runtime_error("GeneticGenerator: factory, initializer and validator are required");
    }
    if (config.populationSize < 2) {
        throw runtime_error("GeneticGenerator: population size must be at least 2");
    }
    if (config.generations < 0) {
        throw runtime_error("GeneticGenerator: generation count cannot be negative");
    }
    if (config.minLength < 1 || config.maxLength < config.minLength) {
        throw runtime_error("GeneticGenerator: invalid sequence length bounds");
    }
    if (config.elitismCount < 0 || config.elitismCount >= config.populationSize) {
        throw runtime_error("GeneticGenerator: elitism count must be in [0, population size)");
    }
}

double GeneticGenerator::evaluate(const FitnessStrategy& fitness,
                                  const TestString&      sequence,
                                  const Spec&            spec,
                                  Archive&               archive) const
{
    const auto existing = archive.find(sequence);
    if (existing != archive.end()) {
        return existing->second;
    }

    const double score = fitness.evaluate(sequence, spec);
    archive.emplace(sequence, score);
    return score;
}

void GeneticGenerator::clampLength(TestString& sequence, const Alphabet& alphabet)
{
    if (static_cast<int>(sequence.size()) > config.maxLength) {
        sequence.resize(config.maxLength);
    }

    const int lastGene = static_cast<int>(alphabet.size()) - 1;
    while (static_cast<int>(sequence.size()) < config.minLength) {
        sequence.push_back(alphabet[rng.uniformInt(0, lastGene)]);
    }
}

Population GeneticGenerator::fittest(const Population& population, int count) const
{
    Population ranked = population;
    // Partial sort is enough: only the leading `count` need to be in order.
    const int taken = min(count, static_cast<int>(ranked.size()));
    partial_sort(ranked.begin(),
                 ranked.begin() + taken,
                 ranked.end(),
                 [](const Individual& lhs, const Individual& rhs) {
                     return lhs.fitness > rhs.fitness;
                 });
    ranked.resize(taken);
    return ranked;
}

GaResult GeneticGenerator::generate(const Spec& spec, int topK)
{
    if (topK < 1) {
        throw runtime_error("GeneticGenerator: topK must be at least 1");
    }

    const Alphabet alphabet = blockNames(spec);
    if (alphabet.empty()) {
        throw runtime_error("GeneticGenerator: the spec declares no API blocks to draw from");
    }

    // Restart the stream so that two runs with the same seed are identical.
    rng = Rng(config.seed);

    // The factory is consulted once, here, and never again during the run.
    const unique_ptr<SelectionStrategy> selection = factory->createSelection();
    const unique_ptr<CrossoverStrategy> crossover = factory->createCrossover();
    const unique_ptr<MutationStrategy>  mutation  = factory->createMutation();
    const unique_ptr<FitnessStrategy>   fitness   = factory->createFitness();
    const double crossoverRate = factory->crossoverRate();
    const double mutationRate  = factory->mutationRate();

    GaResult result;
    Archive  archive;

    // --- generation 0: seed and score -------------------------------------
    Population population;
    population.reserve(config.populationSize);
    for (int attempt = 0;
         attempt < MAX_INITIALIZATION_ATTEMPTS &&
         static_cast<int>(population.size()) < config.populationSize;
         ++attempt) {
        const int missing = config.populationSize - static_cast<int>(population.size());
        for (TestString& sequence : initializer->initialize(alphabet, missing, rng)) {
            if (!validator->isValid(sequence, spec)) {
                ++result.stats.rejectedByValidator;
                continue;
            }
            const double score = evaluate(*fitness, sequence, spec, archive);
            population.push_back(Individual{std::move(sequence), score});
        }
    }
    if (static_cast<int>(population.size()) < config.populationSize) {
        throw runtime_error("GeneticGenerator: could not seed a full population — the validator "
                            "rejected almost everything the initializer produced");
    }

    // --- the generational loop --------------------------------------------
    for (int generation = 0; generation < config.generations; ++generation) {
        Population offspring = fittest(population, config.elitismCount);
        offspring.reserve(config.populationSize);

        while (static_cast<int>(offspring.size()) < config.populationSize) {
            const TestString& parentA = selection->select(population, rng).sequence;
            const TestString& parentB = selection->select(population, rng).sequence;

            TestString childOne = parentA;
            TestString childTwo = parentB;
            if (rng.chance(crossoverRate)) {
                auto children = crossover->crossover(parentA, parentB, rng);
                childOne      = std::move(children.first);
                childTwo      = std::move(children.second);
            }

            // Held by value: parentA/parentB reference the population, which
            // stays alive here, but the fallback has to survive the moves below.
            const TestString fallbacks[2] = {parentA, parentB};
            TestString       children[2]  = {std::move(childOne), std::move(childTwo)};

            for (int i = 0; i < 2 && static_cast<int>(offspring.size()) < config.populationSize;
                 ++i) {
                TestString& child = children[i];
                clampLength(child, alphabet);
                if (rng.chance(mutationRate)) {
                    mutation->mutate(child, alphabet, rng);
                }
                if (!validator->isValid(child, spec)) {
                    ++result.stats.rejectedByValidator;
                    child = fallbacks[i];  // carry the parent through instead
                }

                const double score = evaluate(*fitness, child, spec, archive);
                offspring.push_back(Individual{std::move(child), score});
            }
        }

        population = std::move(offspring);

        double best = population.front().fitness;
        double sum  = 0.0;
        for (const Individual& individual : population) {
            best = max(best, individual.fitness);
            sum += individual.fitness;
        }
        result.stats.bestFitnessPerGeneration.push_back(best);
        result.stats.meanFitnessPerGeneration.push_back(sum / population.size());
    }

    // --- the top K, drawn from everything ever seen ------------------------
    Population candidates;
    candidates.reserve(archive.size());
    for (const auto& entry : archive) {
        candidates.push_back(Individual{entry.first, entry.second});
    }
    sort(candidates.begin(), candidates.end(), [](const Individual& lhs, const Individual& rhs) {
        if (lhs.fitness != rhs.fitness) {
            return lhs.fitness > rhs.fitness;
        }
        // Equally fit: prefer the shorter sequence, then break the remaining
        // ties on content so the ordering is fully determined.
        if (lhs.sequence.size() != rhs.sequence.size()) {
            return lhs.sequence.size() < rhs.sequence.size();
        }
        return lhs.sequence < rhs.sequence;
    });
    if (static_cast<int>(candidates.size()) > topK) {
        candidates.resize(topK);
    }

    result.topK                        = std::move(candidates);
    result.stats.distinctSequencesEvaluated = static_cast<int>(archive.size());
    return result;
}

}  // namespace ga
