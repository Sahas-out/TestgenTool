// ============================================================================
// Smoke test for the genetic test-string generator.
//
// Question it answers: does the module produce well-formed test strings at all?
// It stops there on purpose — nothing here lowers a sequence to an ATC,
// concretizes it, or talks to a backend, so it needs no solver and no running
// server.
//
// Plain asserts and a main(), because the project has no test framework.
//
//     cmake --build build && ./build/ga_smoke_test
// ============================================================================

#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include "specs/LibrarySpec.hh"
#include "teststringGenerator/baseline_ga_factory.hh"
#include "teststringGenerator/genetic_generator.hh"
#include "teststringGenerator/strategies/validation_strategies/always_valid_validator.hh"
#include "teststringGenerator/strategies/population_initialization_strategies/random_population_initializer.hh"

using namespace ga;

static const int TOP_K = 10;

// The permutation under test: the baseline operators, random seeding, and the
// placeholder validator that accepts everything.
static GeneticGenerator makeGenerator(const GaConfig& config) {
    return GeneticGenerator(
        config,
        make_unique<BaselineGaFactory>(),
        make_unique<RandomPopulationInitializer>(config.minLength, config.maxLength),
        make_unique<AlwaysValidValidator>());
}

static set<string> declaredBlockNames(const Spec& spec) {
    set<string> names;
    for (const auto& block : spec.blocks) {
        names.insert(block->name);
    }
    return names;
}

static int distinctBlocks(const TestString& sequence) {
    return static_cast<int>(set<string>(sequence.begin(), sequence.end()).size());
}

static string join(const TestString& sequence) {
    string joined;
    for (size_t i = 0; i < sequence.size(); ++i) {
        joined += (i == 0 ? "" : ", ") + sequence[i];
    }
    return "{" + joined + "}";
}

int main() {
    const GaConfig config;
    auto           spec = makeLibrarySpec();

    cout << "Library spec: " << spec->blocks.size() << " API blocks\n";
    cout << "Running GA: population " << config.populationSize << ", " << config.generations
         << " generations, sequence length " << config.minLength << "-" << config.maxLength
         << ", seed " << config.seed << "\n\n";

    GeneticGenerator generator = makeGenerator(config);
    const GaResult   result    = generator.generate(*spec, TOP_K);

    // --- the generator produced something ----------------------------------
    assert(!result.topK.empty() && "generator returned no test strings");
    assert(static_cast<int>(result.topK.size()) <= TOP_K && "generator returned more than topK");

    // --- every sequence is well formed --------------------------------------
    // The block-name check is the one that really matters downstream: genATC
    // throws on a name the spec does not declare.
    const set<string> declared = declaredBlockNames(*spec);

    for (const Individual& individual : result.topK) {
        const TestString& sequence = individual.sequence;

        assert(!sequence.empty() && "generated an empty test string");
        assert(static_cast<int>(sequence.size()) >= config.minLength && "test string is too short");
        assert(static_cast<int>(sequence.size()) <= config.maxLength && "test string is too long");

        for (const string& name : sequence) {
            assert(declared.count(name) == 1 && "generated a block name the spec does not declare");
        }
    }

    // --- the results are distinct and ranked --------------------------------
    set<TestString> seen;
    for (const Individual& individual : result.topK) {
        assert(seen.insert(individual.sequence).second && "returned the same test string twice");
    }
    for (size_t i = 1; i < result.topK.size(); ++i) {
        assert(result.topK[i - 1].fitness >= result.topK[i].fitness &&
               "results are not ordered by descending fitness");
    }

    // --- the run behaved like a GA ------------------------------------------
    assert(result.stats.bestFitnessPerGeneration.size() == static_cast<size_t>(config.generations));
    assert(result.stats.meanFitnessPerGeneration.size() == static_cast<size_t>(config.generations));
    assert(result.stats.distinctSequencesEvaluated >= config.populationSize);

    // Elitism carries the best individual forward untouched, so the best-fitness
    // curve can never dip. If it does, elitism is broken.
    for (size_t i = 1; i < result.stats.bestFitnessPerGeneration.size(); ++i) {
        assert(result.stats.bestFitnessPerGeneration[i] >=
                   result.stats.bestFitnessPerGeneration[i - 1] &&
               "best fitness decreased between generations — elitism is not holding");
    }

    // --- the same seed gives the same run -----------------------------------
    GeneticGenerator repeat      = makeGenerator(config);
    const GaResult   repeatedRun = repeat.generate(*spec, TOP_K);

    assert(repeatedRun.topK.size() == result.topK.size() && "rerun produced a different result");
    for (size_t i = 0; i < result.topK.size(); ++i) {
        assert(repeatedRun.topK[i].sequence == result.topK[i].sequence &&
               "rerun with the same seed produced different test strings");
        assert(repeatedRun.topK[i].fitness == result.topK[i].fitness &&
               "rerun with the same seed produced different fitness");
    }

    // --- report --------------------------------------------------------------
    cout << "Top " << result.topK.size() << " test strings:\n";
    for (size_t i = 0; i < result.topK.size(); ++i) {
        const Individual& individual = result.topK[i];
        cout << "  " << (i + 1) << ". fitness " << individual.fitness << "  ("
             << individual.sequence.size() << " calls, " << distinctBlocks(individual.sequence)
             << " distinct)\n";
        cout << "     " << join(individual.sequence) << "\n";
    }

    cout << "\nBest fitness per generation:\n  ";
    for (const double best : result.stats.bestFitnessPerGeneration) {
        cout << best << " ";
    }
    cout << "\nMean fitness per generation:\n  ";
    for (const double mean : result.stats.meanFitnessPerGeneration) {
        cout << mean << " ";
    }

    cout << "\n\nDistinct sequences evaluated: " << result.stats.distinctSequencesEvaluated << "\n";
    cout << "Rejected by validator: " << result.stats.rejectedByValidator
         << " (expected 0 while the always-valid validator is in use)\n";

    cout << "\nAll assertions passed.\n";
    return 0;
}
