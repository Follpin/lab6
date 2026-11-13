#include "game_state.h"
#include <format>

namespace Game {

GameState::GameState(Number a, Number b)
    : a_(a), b_(b), initialSum_(a + b) {
    if (a == 0 || b == 0) {
        throw std::invalid_argument("Initial numbers must be strictly positive (> 0).");
    }
}

bool GameState::isActionValid(const Action& action, std::optional<ActionType> lastAction) const noexcept {
    if (lastAction.has_value() && action.type == *lastAction) {
        return false;
    }

    if (action.type == ActionType::DivideThree) {
        if (action.target == TargetNumber::A) {
            return (a_ % 3 == 0);
        } else {
            return (b_ % 3 == 0);
        }
    }

    return true;
}

std::vector<Action> GameState::getValidActions(std::optional<ActionType> lastAction) const {
    std::vector<Action> valid;
    std::vector<ActionType> allTypes = { ActionType::AddOne, ActionType::MultiplyTwo, ActionType::DivideThree };
    std::vector<TargetNumber> allTargets = { TargetNumber::A, TargetNumber::B };

    for (auto type : allTypes) {
        for (auto target : allTargets) {
            Action act{ type, target };
            if (isActionValid(act, lastAction)) {
                valid.push_back(act);
            }
        }
    }
    return valid;
}

void GameState::applyAction(const Action& action) {
    switch (action.type) {
        case ActionType::AddOne:
            if (action.target == TargetNumber::A) ++a_;
            else ++b_;
            break;
        case ActionType::MultiplyTwo:
            if (action.target == TargetNumber::A) a_ *= 2;
            else b_ *= 2;
            break;
        case ActionType::DivideThree:
            if (action.target == TargetNumber::A) {
                if (a_ % 3 != 0) {
                    throw std::logic_error(std::format("Cannot divide by 3: number a ({}) is not divisible by 3", a_));
                }
                a_ /= 3;
            } else {
                if (b_ % 3 != 0) {
                    throw std::logic_error(std::format("Cannot divide by 3: number b ({}) is not divisible by 3", b_));
                }
                b_ /= 3;
            }
            break;
    }
}

}
