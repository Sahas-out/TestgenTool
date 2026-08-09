#pragma once

// ============================================================================
// The genetic algorithm itself.
//
// Assembled from three injected pieces:
//   - a GaFactory, which supplies the four operators and their rates,
//   - a PopulationInitializer, which seeds the first generation,
//   - a TestStringValidator, which decides what is worth keeping.
//
// The loop below names none of the concrete strategies, so a new permutation
// changes the construction site and nothing in this file.
// ============================================================================

#include <map>
#include <memory>

#include "teststringGenerator/ga_config.hh"
#include "teststringGenerator/ga_factory.hh"
#include "teststringGenerator/rng.hh"
#include "teststringGenerator/strategies/population_initializer.hh"
#include "teststringGenerator/strategies/teststring_validator.hh"
#include "teststringGenerator/teststring_generator.hh"

namespace ga {

class GeneticGenerator : public TestStringGenerator
{
public:
    GeneticGenerator(GaConfig                          config,
                     unique_ptr<GaFactory>             factory,
                     unique_ptr<PopulationInitializer> initializer,
                     unique_ptr<TestStringValidator>   validator);

    GaResult generate(const Spec& spec, int topK) override;

private:
    // Every distinct sequence the run has ever scored, with its fitness.
    //
    // Serves two purposes at once: it memoises fitness, so a sequence that
    // reappears is never re-scored (which will matter a great deal once fitness
    // does real work), and it is the pool the final top K is drawn from, so a
    // good sequence found early is not lost when a later generation drifts away
    // from it.
    using Archive = map<TestString, double>;

    // Scores a sequence, or returns the score already on file for it.
    double evaluate(const FitnessStrategy& fitness,
                    const TestString&      sequence,
                    const Spec&            spec,
                    Archive&               archive) const;

    // Forces a sequence into [minLength, maxLength]: truncate if too long,
    // extend with random blocks if too short.
    //
    // Lives here rather than in the crossover operator so that every present
    // and future crossover obeys the bounds without having to remember to.
    void clampLength(TestString& sequence, const Alphabet& alphabet);

    // The fittest individuals of a population, in descending fitness order.
    Population fittest(const Population& population, int count) const;

    GaConfig                          config;
    unique_ptr<GaFactory>             factory;
    unique_ptr<PopulationInitializer> initializer;
    unique_ptr<TestStringValidator>   validator;
    Rng                               rng;
};

}  // namespace ga
