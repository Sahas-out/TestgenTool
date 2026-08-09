#pragma once

#include "teststringGenerator/strategies/crossover_strategy.hh"

namespace ga {

// Prefix-suffix (single point) crossover.
//
// Cut points are chosen independently in each parent, because sequences vary in
// length and forcing a shared cut point would bias children toward the shorter
// parent's length:
//
//     child1 = parentA[0..i) + parentB[j..)
//     child2 = parentB[0..j) + parentA[i..)
//
// Children may come out shorter or longer than either parent; GeneticGenerator
// clamps them into the configured length bounds afterwards.
class SinglePointCrossover : public CrossoverStrategy
{
public:
    pair<TestString, TestString> crossover(const TestString& parentA,
                                           const TestString& parentB,
                                           Rng&              rng) const override;
};

}  // namespace ga
