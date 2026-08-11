#pragma once

#include "teststringGenerator/strategies/selection_strategies/selection_strategy.hh"

namespace ga {

// k-way tournament: sample k individuals uniformly at random (with
// replacement) and return the fittest of them.
//
// k controls selection pressure — k=1 is a random walk, large k crowds out
// everything but the current best. It is an operator parameter, so it is
// supplied by the concrete GaFactory rather than by GaConfig.
class TournamentSelection : public SelectionStrategy {
public:
    explicit TournamentSelection(int tournamentSize);

    const Individual& select(const Population& population, Rng& rng) const override;

private:
    int tournamentSize;
};

}  // namespace ga
