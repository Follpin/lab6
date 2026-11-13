#include "strategy.h"
#include <algorithm>
#include <stdexcept>
#include <chrono>

namespace Game {

Action GreedyStrategy::chooseAction(const GameState& state, std::optional<ActionType> lastAction) {
    auto validActions = state.getValidActions(lastAction);
    if (validActions.empty()) {
        throw std::runtime_error("No valid actions available for greedy strategy.");
    }

    for (const auto& act : validActions) {
        GameState testState = state;
        testState.applyAction(act);
        if (testState.isWinning()) {
            return act;
        }
    }

    Action bestAction = validActions[0];
    Number bestSum = 0;

    for (const auto& act : validActions) {
        GameState testState = state;
        testState.applyAction(act);
        if (testState.currentSum() > bestSum) {
            bestSum = testState.currentSum();
            bestAction = act;
        }
    }

    return bestAction;
}

RandomStrategy::RandomStrategy(std::optional<uint64_t> seed) {
    if (seed.has_value()) {
        rng_.seed(*seed);
    } else {
        std::random_device rd;
        rng_.seed(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count());
    }
}

Action RandomStrategy::chooseAction(const GameState& state, std::optional<ActionType> lastAction) {
    auto validActions = state.getValidActions(lastAction);
    if (validActions.empty()) {
        throw std::runtime_error("No valid actions available for random strategy.");
    }

    std::uniform_int_distribution<size_t> dist(0, validActions.size() - 1);
    return validActions[dist(rng_)];
}

}
