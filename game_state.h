#pragma once
#include "types.h"
#include "action.h"
#include <vector>
#include <optional>
#include <stdexcept>

namespace Game {

class GameState {
public:
    GameState() : a_(1), b_(1), initialSum_(2) {}
    GameState(Number a, Number b);

    Number a() const noexcept { return a_; }
    Number b() const noexcept { return b_; }
    Number initialSum() const noexcept { return initialSum_; }
    Number currentSum() const noexcept { return a_ + b_; }
    Number targetSum() const noexcept { return initialSum_ * 10; }

    bool isWinning() const noexcept {
        return currentSum() >= targetSum();
    }

    bool isActionValid(const Action& action, std::optional<ActionType> lastAction) const noexcept;
    std::vector<Action> getValidActions(std::optional<ActionType> lastAction) const;
    void applyAction(const Action& action);

private:
    Number a_;
    Number b_;
    Number initialSum_;
};

}
