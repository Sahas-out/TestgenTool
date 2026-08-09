#include "teststringGenerator/strategies/single_point_crossover.hh"

namespace ga {

pair<TestString, TestString> SinglePointCrossover::crossover(const TestString& parentA,
                                                             const TestString& parentB,
                                                             Rng&              rng) const
{
    // Cut points range over [0, size], so a cut may take all or none of a
    // parent — that is what lets children change length.
    const int cutA = rng.uniformInt(0, static_cast<int>(parentA.size()));
    const int cutB = rng.uniformInt(0, static_cast<int>(parentB.size()));

    TestString childOne(parentA.begin(), parentA.begin() + cutA);
    childOne.insert(childOne.end(), parentB.begin() + cutB, parentB.end());

    TestString childTwo(parentB.begin(), parentB.begin() + cutB);
    childTwo.insert(childTwo.end(), parentA.begin() + cutA, parentA.end());

    return {std::move(childOne), std::move(childTwo)};
}

}  // namespace ga
