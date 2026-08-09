#pragma once

#include "teststringGenerator/strategies/teststring_validator.hh"

namespace ga {

// Accepts every sequence.
//
// The placeholder for this first pass, so the GA loop can be brought up and
// exercised before feasibility checking exists. The real implementation will
// simulate the abstract state: lower the sequence, replace each assert with the
// state update it implies, and check every assume condition holds — a sequence
// whose assume fails is not a runnable test.
class AlwaysValidValidator : public TestStringValidator
{
public:
    bool isValid(const TestString& sequence, const Spec& spec) const override;
};

}  // namespace ga
