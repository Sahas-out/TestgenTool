#pragma once

// ============================================================================
// Permutation #1: tournament selection, prefix-suffix crossover, block
// replacement mutation, block-coverage fitness.
//
// "Baseline" rather than "concrete" so later permutations can be named for what
// makes them different (UniformCrossoverGaFactory, RankSelectionGaFactory, ...)
// instead of colliding on a generic name.
//
// Every operator parameter lives here, so a differently tuned variant of the
// same four operators is just a different construction:
//
//     BaselineGaFactory{}                       // the defaults below
//     BaselineGaFactory{0.5, 0.4, 5, 1.0}       // lazier crossover, hotter
//                                               // mutation, more pressure
// ============================================================================

#include "teststringGenerator/ga_factory.hh"

namespace ga {

class BaselineGaFactory : public GaFactory
{
public:
    explicit BaselineGaFactory(double crossoverProbability = 0.8,
                               double mutationProbability  = 0.2,
                               int    tournamentSize       = 3,
                               double coverageWeight       = 1.0);

    unique_ptr<SelectionStrategy> createSelection() const override;
    unique_ptr<CrossoverStrategy> createCrossover() const override;
    unique_ptr<MutationStrategy>  createMutation() const override;
    unique_ptr<FitnessStrategy>   createFitness() const override;

    double crossoverRate() const override;
    double mutationRate() const override;

private:
    double crossoverProbability;
    double mutationProbability;
    int    tournamentSize;
    double coverageWeight;
};

}  // namespace ga
