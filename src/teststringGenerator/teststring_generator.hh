#pragma once

// ============================================================================
// The public interface of this module: something that, given a specification,
// produces test strings worth running.
//
// The Spec is a parameter rather than a constructor field, so one configured
// generator can be pointed at the Library, Ecommerce and Restaurant specs in
// turn.
// ============================================================================

#include "teststringGenerator/ga_types.hh"

namespace ga {

class TestStringGenerator {
public:
    virtual ~TestStringGenerator() = default;

    // Returns at most topK sequences, best first, plus statistics about the run.
    virtual GaResult generate(const Spec& spec, int topK) = 0;
};

}  // namespace ga
