#pragma once

#include "teststringGenerator/ga_types.hh"
#include "teststringGenerator/rng.hh"

namespace ga {

// Chooses a parent from the current population.
//
// Returns a reference into the population, so the caller must not outlive or
// modify it while holding the result.
class SelectionStrategy
{
public:
    virtual ~SelectionStrategy() = default;

    virtual const Individual& select(const Population& population, Rng& rng) const = 0;
};

}  // namespace ga
