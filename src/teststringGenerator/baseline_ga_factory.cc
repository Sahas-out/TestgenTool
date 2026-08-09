#include "teststringGenerator/baseline_ga_factory.hh"

#include <stdexcept>

#include "teststringGenerator/strategies/block_coverage_fitness.hh"
#include "teststringGenerator/strategies/block_replacement_mutation.hh"
#include "teststringGenerator/strategies/single_point_crossover.hh"
#include "teststringGenerator/strategies/tournament_selection.hh"

namespace ga {

BaselineGaFactory::BaselineGaFactory(double crossoverProbability,
                                     double mutationProbability,
                                     int    tournamentSize,
                                     double coverageWeight)
    : crossoverProbability(crossoverProbability),
      mutationProbability(mutationProbability),
      tournamentSize(tournamentSize),
      coverageWeight(coverageWeight)
{
    if (crossoverProbability < 0.0 || crossoverProbability > 1.0) {
        throw runtime_error("BaselineGaFactory: crossover probability must be in [0, 1]");
    }
    if (mutationProbability < 0.0 || mutationProbability > 1.0) {
        throw runtime_error("BaselineGaFactory: mutation probability must be in [0, 1]");
    }
}

unique_ptr<SelectionStrategy> BaselineGaFactory::createSelection() const
{
    return make_unique<TournamentSelection>(tournamentSize);
}

unique_ptr<CrossoverStrategy> BaselineGaFactory::createCrossover() const
{
    return make_unique<SinglePointCrossover>();
}

unique_ptr<MutationStrategy> BaselineGaFactory::createMutation() const
{
    return make_unique<BlockReplacementMutation>();
}

unique_ptr<FitnessStrategy> BaselineGaFactory::createFitness() const
{
    return make_unique<BlockCoverageFitness>(coverageWeight);
}

double BaselineGaFactory::crossoverRate() const { return crossoverProbability; }

double BaselineGaFactory::mutationRate() const { return mutationProbability; }

}  // namespace ga
