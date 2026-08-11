#include "teststringGenerator/genetic_generator.hh"

#include <algorithm>
#include <stdexcept>

namespace ga {

// How many times seeding may ask the initializer for a fresh batch before
// giving up. Only relevant once a real validator can reject sequences; it exists
// so a validator that refuses everything fails loudly instead of looping forever.
static const int MAX_SEEDING_ATTEMPTS = 20;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

GeneticGenerator::GeneticGenerator(GaConfig                          config,
                                   unique_ptr<GaFactory>             factory,
                                   unique_ptr<PopulationInitializer> initializer,
                                   unique_ptr<TestStringValidator>   validator)
    : config(config),
      factory(std::move(factory)),
      initializer(std::move(initializer)),
      validator(std::move(validator)),
      rng(config.seed) {
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

// ---------------------------------------------------------------------------
// The run
// ---------------------------------------------------------------------------

GaResult GeneticGenerator::generate(const Spec& spec, int topK) {
    if (topK < 1) {
        throw runtime_error("GeneticGenerator: topK must be at least 1");
    }

    const Alphabet alphabet = blockNames(spec);
    if (alphabet.empty()) {
        throw runtime_error("GeneticGenerator: the spec declares no API blocks to draw from");
    }

    // Restart the stream so that two runs with the same seed are identical.
    rng = Rng(config.seed);

    const Operators operators = buildOperators();

    GaResult   result;
    Archive    archive;
    Population population = seedPopulation(spec, alphabet, operators, archive, result.stats);

    for (int generation = 0; generation < config.generations; ++generation) {
        population = breedNextGeneration(population, spec, alphabet, operators, archive,
                                         result.stats);
    }

    result.topK                             = rankArchive(archive, topK);
    result.stats.distinctSequencesEvaluated = static_cast<int>(archive.size());
    return result;
}

GeneticGenerator::Operators GeneticGenerator::buildOperators() const {
    Operators operators;
    operators.selection     = factory->createSelection();
    operators.crossover     = factory->createCrossover();
    operators.mutation      = factory->createMutation();
    operators.fitness       = factory->createFitness();
    operators.crossoverRate = factory->crossoverRate();
    operators.mutationRate  = factory->mutationRate();
    return operators;
}

Population GeneticGenerator::seedPopulation(const Spec&      spec,
                                            const Alphabet&  alphabet,
                                            const Operators& operators,
                                            Archive&         archive,
                                            GaRunStats&      stats) {
    Population population;
    population.reserve(config.populationSize);

    // Ask for as many as are still missing, repeatedly, because the validator
    // may throw some of each batch away.
    for (int attempt = 0;
         attempt < MAX_SEEDING_ATTEMPTS &&
         static_cast<int>(population.size()) < config.populationSize;
         ++attempt) {
        const int missing = config.populationSize - static_cast<int>(population.size());

        for (TestString& sequence : initializer->initialize(alphabet, missing, rng)) {
            if (!validator->isValid(sequence, spec)) {
                ++stats.rejectedByValidator;
                continue;
            }
            const double score = evaluate(*operators.fitness, sequence, spec, archive);
            population.push_back(Individual{std::move(sequence), score});
        }
    }

    if (static_cast<int>(population.size()) < config.populationSize) {
        throw runtime_error("GeneticGenerator: could not seed a full population — the validator "
                            "rejected almost everything the initializer produced");
    }
    return population;
}

Population GeneticGenerator::breedNextGeneration(const Population& population,
                                                 const Spec&       spec,
                                                 const Alphabet&   alphabet,
                                                 const Operators&  operators,
                                                 Archive&          archive,
                                                 GaRunStats&       stats) {
    // The elites carry through untouched, which is what keeps the best-fitness
    // curve from ever dipping.
    Population offspring = fittest(population, config.elitismCount);
    offspring.reserve(config.populationSize);

    while (static_cast<int>(offspring.size()) < config.populationSize) {
        const TestString& parentA = operators.selection->select(population, rng).sequence;
        const TestString& parentB = operators.selection->select(population, rng).sequence;

        // Without crossover the parents pass through as clones and only
        // mutation varies them.
        TestString children[2] = {parentA, parentB};
        if (rng.chance(operators.crossoverRate)) {
            auto crossed = operators.crossover->crossover(parentA, parentB, rng);
            children[0]  = std::move(crossed.first);
            children[1]  = std::move(crossed.second);
        }

        // Copies, because a rejected child falls back to its parent and the
        // references above point into a population we are reading from.
        const TestString fallbacks[2] = {parentA, parentB};

        for (int i = 0; i < 2 && static_cast<int>(offspring.size()) < config.populationSize; ++i) {
            TestString& child = children[i];

            clampLength(child, alphabet);
            if (rng.chance(operators.mutationRate)) {
                operators.mutation->mutate(child, alphabet, rng);
            }
            if (!validator->isValid(child, spec)) {
                ++stats.rejectedByValidator;
                child = fallbacks[i];  // carry the parent through instead
            }

            const double score = evaluate(*operators.fitness, child, spec, archive);
            offspring.push_back(Individual{std::move(child), score});
        }
    }

    double best = offspring.front().fitness;
    double sum  = 0.0;
    for (const Individual& individual : offspring) {
        best = max(best, individual.fitness);
        sum += individual.fitness;
    }
    stats.bestFitnessPerGeneration.push_back(best);
    stats.meanFitnessPerGeneration.push_back(sum / offspring.size());

    return offspring;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

double GeneticGenerator::evaluate(const FitnessStrategy& fitness,
                                  const TestString&      sequence,
                                  const Spec&            spec,
                                  Archive&               archive) const {
    const auto existing = archive.find(sequence);
    if (existing != archive.end()) {
        return existing->second;
    }

    const double score = fitness.evaluate(sequence, spec);
    archive.emplace(sequence, score);
    return score;
}

void GeneticGenerator::clampLength(TestString& sequence, const Alphabet& alphabet) {
    if (static_cast<int>(sequence.size()) > config.maxLength) {
        sequence.resize(config.maxLength);
    }

    const int lastGene = static_cast<int>(alphabet.size()) - 1;
    while (static_cast<int>(sequence.size()) < config.minLength) {
        sequence.push_back(alphabet[rng.uniformInt(0, lastGene)]);
    }
}

Population GeneticGenerator::fittest(const Population& population, int count) const {
    Population ranked = population;

    // Partial sort is enough: only the leading `count` need to be in order.
    const int taken = min(count, static_cast<int>(ranked.size()));
    partial_sort(ranked.begin(), ranked.begin() + taken, ranked.end(),
                 [](const Individual& lhs, const Individual& rhs) {
                     return lhs.fitness > rhs.fitness;
                 });
    ranked.resize(taken);
    return ranked;
}

vector<Individual> GeneticGenerator::rankArchive(const Archive& archive, int topK) const {
    vector<Individual> candidates;
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
    return candidates;
}

}  // namespace ga
