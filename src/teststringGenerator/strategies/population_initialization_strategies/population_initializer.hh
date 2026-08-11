#pragma once

#include <vector>

#include "teststringGenerator/ga_types.hh"
#include "teststringGenerator/rng.hh"

namespace ga {

// Produces the starting sequences of a run.
//
// Injected into GeneticGenerator separately from the GaFactory: how a run is
// seeded is orthogonal to which crossover/mutation/selection/fitness operators
// it uses, and the two should be swappable independently.
class PopulationInitializer {
public:
    virtual ~PopulationInitializer() = default;

    virtual vector<TestString> initialize(const Alphabet& alphabet, int count, Rng& rng) const = 0;
};

}  // namespace ga
