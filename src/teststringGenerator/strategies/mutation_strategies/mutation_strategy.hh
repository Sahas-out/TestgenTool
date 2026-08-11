#pragma once

#include "teststringGenerator/ga_types.hh"
#include "teststringGenerator/rng.hh"

namespace ga {

// Perturbs a single sequence in place.
//
// Mutation and population initialisation are the only two places block names
// are created. Both draw exclusively from the alphabet, which is what
// guarantees every generated name resolves in genATC.
class MutationStrategy {
public:
    virtual ~MutationStrategy() = default;

    virtual void mutate(TestString& sequence, const Alphabet& alphabet, Rng& rng) const = 0;
};

}  // namespace ga
