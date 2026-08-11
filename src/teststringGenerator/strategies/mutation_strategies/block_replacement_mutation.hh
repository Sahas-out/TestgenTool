#pragma once

#include "teststringGenerator/strategies/mutation_strategies/mutation_strategy.hh"

namespace ga {

// Replaces one randomly chosen position in the sequence with a random block
// name from the alphabet.
//
// Length-preserving, so it explores which blocks appear and in what order but
// never how many — crossover is what varies length.
class BlockReplacementMutation : public MutationStrategy {
public:
    void mutate(TestString& sequence, const Alphabet& alphabet, Rng& rng) const override;
};

}  // namespace ga
