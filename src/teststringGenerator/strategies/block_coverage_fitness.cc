#include "teststringGenerator/strategies/block_coverage_fitness.hh"

#include <set>

namespace ga {

BlockCoverageFitness::BlockCoverageFitness(double weight) : weight(weight) {}

double BlockCoverageFitness::evaluate(const TestString& sequence, const Spec& spec) const
{
    (void)spec;  // coverage is measured on the sequence alone

    const set<string> covered(sequence.begin(), sequence.end());
    return weight * static_cast<double>(covered.size());
}

}  // namespace ga
