#pragma once

#include "teststringGenerator/ga_types.hh"

namespace ga {

// Decides whether a sequence is worth keeping at all.
//
// Takes the Spec even though the current always-true implementation ignores it:
// the abstract-state simulator that replaces it will lower the sequence against
// the spec and check each assume condition, so the interface is shaped for that
// now rather than changed later.
//
// Like PopulationInitializer, this is injected separately from the GaFactory —
// running one operator permutation against both a trivial and a real validator
// is exactly the comparison this module is built to support.
class TestStringValidator {
public:
    virtual ~TestStringValidator() = default;

    virtual bool isValid(const TestString& sequence, const Spec& spec) const = 0;
};

}  // namespace ga
