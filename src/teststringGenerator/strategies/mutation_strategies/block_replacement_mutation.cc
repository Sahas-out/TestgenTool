#include "teststringGenerator/strategies/mutation_strategies/block_replacement_mutation.hh"

namespace ga {

void BlockReplacementMutation::mutate(TestString&     sequence,
                                      const Alphabet& alphabet,
                                      Rng&            rng) const {
    // Nothing to replace, or nothing to replace it with.
    if (sequence.empty() || alphabet.empty()) {
        return;
    }

    const int position = rng.uniformInt(0, static_cast<int>(sequence.size()) - 1);
    const int gene     = rng.uniformInt(0, static_cast<int>(alphabet.size()) - 1);
    sequence[position] = alphabet[gene];
}

}  // namespace ga
