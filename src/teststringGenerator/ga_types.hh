#pragma once

// ============================================================================
// Core value types for the genetic test-string generator.
//
// A test string is an ordered sequence of API block names drawn from a Spec.
// The GA searches the space of such sequences, so the types here are all thin
// wrappers around vector<string> plus the bookkeeping the loop needs.
//
// Everything in this module lives in namespace ga: names like Individual and
// SelectionStrategy are too generic to sit in the global namespace next to
// Spec and Program.
// ============================================================================

#include <string>
#include <vector>

#include "ast/ast.hh"

using namespace std;

namespace ga {

// An ordered sequence of API block names, e.g. {"saveBookOk", "getBookByCodeOk"}.
// This is exactly what genATC(spec, testString) consumes downstream.
using TestString = vector<string>;

// Every block name declared by a Spec — the alphabet the GA draws genes from.
// Sequences may repeat a name: saving two books then listing them is a
// legitimate test, so nothing here enforces distinctness.
using Alphabet = vector<string>;

// One candidate solution: a sequence and the score the fitness strategy gave it.
struct Individual {
    TestString sequence;
    double     fitness = 0.0;
};

using Population = vector<Individual>;

// Per-run instrumentation. The point of the abstract-factory design is to try
// different operator permutations, and "which permutation converged faster" is
// unanswerable from the returned sequences alone — hence the fitness curves.
struct GaRunStats {
    vector<double> bestFitnessPerGeneration;
    vector<double> meanFitnessPerGeneration;
    int distinctSequencesEvaluated = 0;
    int rejectedByValidator        = 0;  // stays 0 while AlwaysValidValidator is in use
};

// What generate() hands back: the top K sequences with their scores, plus how
// the run got there.
struct GaResult {
    vector<Individual> topK;  // best first
    GaRunStats         stats;
};

// The block names of a Spec, in declaration order.
//
// This is the only place in the module that knows how Spec stores its blocks,
// so a change to Spec's layout lands here and nowhere else.
Alphabet blockNames(const Spec& spec);

}  // namespace ga
