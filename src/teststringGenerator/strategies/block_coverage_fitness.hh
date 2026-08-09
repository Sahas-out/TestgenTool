#pragma once

#include "teststringGenerator/strategies/fitness_strategy.hh"

namespace ga {

// fitness = weight * (number of distinct API blocks the sequence covers)
//
// Repeats score nothing extra by construction. Sequences are allowed to repeat
// a block, so this is the selection pressure that squeezes pointless repetition
// out on its own, without a hard constraint doing it.
//
// Note this rewards breadth only: it says nothing about whether the sequence is
// actually executable in order. That signal comes from the validator and from
// richer fitness functions later — this is the baseline to compare them against.
class BlockCoverageFitness : public FitnessStrategy
{
public:
    explicit BlockCoverageFitness(double weight);

    double evaluate(const TestString& sequence, const Spec& spec) const override;

private:
    double weight;
};

}  // namespace ga
