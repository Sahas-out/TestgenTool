#pragma once

#include "teststringGenerator/strategies/population_initialization_strategies/population_initializer.hh"

namespace ga {

// Seeds the run with uniformly random sequences: each one gets a random length
// in [minLength, maxLength] and each position a random block name. Repeats are
// permitted.
class RandomPopulationInitializer : public PopulationInitializer {
public:
    RandomPopulationInitializer(int minLength, int maxLength);

    vector<TestString> initialize(const Alphabet& alphabet, int count, Rng& rng) const override;

private:
    int minLength;
    int maxLength;
};

}  // namespace ga
