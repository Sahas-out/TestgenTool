#pragma once

// ============================================================================
// The abstract factory that defines a GA permutation.
//
// A permutation is a choice of the four operators TOGETHER WITH the rates they
// run at: single-point crossover at 0.9 is a genuinely different variant from
// the same operator at 0.5. Keeping the rates here rather than in GaConfig
// means one factory object fully describes a variant, so a variant can be named,
// passed around and compared as a unit.
//
// GeneticGenerator only ever talks to this interface — it never names a
// concrete strategy. Trying a new permutation is one new subclass and nothing
// else; adding a fifth strategy axis is one more method here.
//
// Note what is deliberately NOT produced by this factory: the population
// initializer and the test-string validator. Those are orthogonal axes, injected
// into GeneticGenerator separately, so that one permutation can be run against
// several validators without needing a factory per combination.
// ============================================================================

#include <memory>

#include "teststringGenerator/strategies/crossover_strategy.hh"
#include "teststringGenerator/strategies/fitness_strategy.hh"
#include "teststringGenerator/strategies/mutation_strategy.hh"
#include "teststringGenerator/strategies/selection_strategy.hh"

using namespace std;

namespace ga {

class GaFactory
{
public:
    virtual ~GaFactory() = default;

    virtual unique_ptr<SelectionStrategy> createSelection() const = 0;
    virtual unique_ptr<CrossoverStrategy> createCrossover() const = 0;
    virtual unique_ptr<MutationStrategy>  createMutation() const  = 0;
    virtual unique_ptr<FitnessStrategy>   createFitness() const   = 0;

    // Probability that a selected pair of parents is recombined rather than
    // copied through unchanged.
    virtual double crossoverRate() const = 0;

    // Probability that an individual offspring is mutated.
    virtual double mutationRate() const = 0;
};

}  // namespace ga
