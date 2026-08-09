#pragma once

// ============================================================================
// Run parameters for a GA run: how big a population, how long to search, how
// long a sequence may be.
//
// Operator parameters (crossover/mutation probabilities, tournament size,
// fitness weights) deliberately do NOT live here — they belong to the concrete
// GaFactory, because a permutation is "these operators AT these rates". Keeping
// them apart means a factory alone fully describes a variant, and a config that
// disagrees with a factory cannot silently produce a variant nobody meant to
// test. See ga_factory.hh.
// ============================================================================

namespace ga {

struct GaConfig
{
    int populationSize = 50;
    int generations    = 30;  // TERMINATION: a fixed count, for now.

    // Length bounds on a generated sequence. maxLength sits above the typical
    // hand-written suite string (most are 2-6 blocks) and below the ~20-block
    // alphabet of the existing specs, so coverage fitness stays a real gradient
    // instead of saturating in the first generation.
    int minLength = 2;
    int maxLength = 8;

    // How many of the fittest survive each generation untouched. Guarantees the
    // best-fitness curve is monotonically non-decreasing.
    int elitismCount = 2;

    // Fixed rather than seeded from random_device: a run that cannot be
    // reproduced cannot be debugged, and the smoke test asserts determinism.
    unsigned seed = 42u;
};

}  // namespace ga
