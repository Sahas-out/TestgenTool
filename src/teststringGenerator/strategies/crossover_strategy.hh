#pragma once

#include <utility>

#include "teststringGenerator/ga_types.hh"
#include "teststringGenerator/rng.hh"

namespace ga {

// Recombines two parent sequences into two children.
//
// Implementations do NOT enforce length bounds: GeneticGenerator clamps every
// child to [minLength, maxLength] after the call, so a new crossover operator
// gets that for free and cannot get it wrong.
class CrossoverStrategy
{
public:
    virtual ~CrossoverStrategy() = default;

    virtual pair<TestString, TestString> crossover(const TestString& parentA,
                                                   const TestString& parentB,
                                                   Rng&              rng) const = 0;
};

}  // namespace ga
