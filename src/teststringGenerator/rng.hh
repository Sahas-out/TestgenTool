#pragma once

// ============================================================================
// The single source of randomness for a GA run.
//
// GeneticGenerator owns one Rng, seeded from GaConfig, and passes it by
// reference into every strategy call. Strategies therefore hold no random state
// of their own: they stay reusable across runs, and one seed reproduces an
// entire run exactly.
// ============================================================================

#include <random>

using namespace std;

namespace ga {

class Rng {
public:
    explicit Rng(unsigned seed) : engine(seed) {}

    // Uniform in [lo, hi], inclusive at both ends.
    int uniformInt(int lo, int hi) {
        uniform_int_distribution<int> dist(lo, hi);
        return dist(engine);
    }

    // Uniform in [0, 1).
    double uniformReal() {
        uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    // True with the given probability.
    bool chance(double probability) {
        return uniformReal() < probability;
    }

private:
    mt19937 engine;
};

}  // namespace ga
