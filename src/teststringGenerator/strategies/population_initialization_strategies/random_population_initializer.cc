#include "teststringGenerator/strategies/population_initialization_strategies/random_population_initializer.hh"

#include <stdexcept>

namespace ga {

RandomPopulationInitializer::RandomPopulationInitializer(int minLength, int maxLength)
    : minLength(minLength), maxLength(maxLength) {
    if (minLength < 1) {
        throw runtime_error("RandomPopulationInitializer: minimum length must be at least 1");
    }
    if (maxLength < minLength) {
        throw runtime_error("RandomPopulationInitializer: maximum length is below minimum length");
    }
}

vector<TestString> RandomPopulationInitializer::initialize(const Alphabet& alphabet,
                                                           int             count,
                                                           Rng&            rng) const {
    if (alphabet.empty()) {
        throw runtime_error("RandomPopulationInitializer: the spec declares no API blocks");
    }

    const int lastGene = static_cast<int>(alphabet.size()) - 1;

    vector<TestString> sequences;
    sequences.reserve(count);
    for (int i = 0; i < count; ++i) {
        const int length = rng.uniformInt(minLength, maxLength);

        TestString sequence;
        sequence.reserve(length);
        for (int position = 0; position < length; ++position) {
            sequence.push_back(alphabet[rng.uniformInt(0, lastGene)]);
        }
        sequences.push_back(std::move(sequence));
    }
    return sequences;
}

}  // namespace ga
