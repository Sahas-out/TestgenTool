#include "teststringGenerator/strategies/tournament_selection.hh"

#include <stdexcept>

namespace ga {

TournamentSelection::TournamentSelection(int tournamentSize) : tournamentSize(tournamentSize)
{
    if (tournamentSize < 1) {
        throw runtime_error("TournamentSelection: tournament size must be at least 1");
    }
}

const Individual& TournamentSelection::select(const Population& population, Rng& rng) const
{
    if (population.empty()) {
        throw runtime_error("TournamentSelection: cannot select from an empty population");
    }

    const int lastIndex = static_cast<int>(population.size()) - 1;

    const Individual* best = &population[rng.uniformInt(0, lastIndex)];
    for (int i = 1; i < tournamentSize; ++i) {
        const Individual& challenger = population[rng.uniformInt(0, lastIndex)];
        if (challenger.fitness > best->fitness) {
            best = &challenger;
        }
    }
    return *best;
}

}  // namespace ga
