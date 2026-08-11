#pragma once

#include "teststringGenerator/ga_types.hh"

namespace ga {

// Scores a sequence: higher is better.
//
// Takes the Spec because richer fitness functions need it — coverage relative
// to the whole spec, precondition reasoning, and so on.
class FitnessStrategy {
public:
    virtual ~FitnessStrategy() = default;

    virtual double evaluate(const TestString& sequence, const Spec& spec) const = 0;
};

}  // namespace ga
