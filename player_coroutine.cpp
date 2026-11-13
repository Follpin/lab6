#include "player_coroutine.h"
#include <iostream>
#include <format>
#include <stdexcept>

namespace Game {

TurnResult PlayerCoroutine::playTurn(const GameState& currentState) {
    if (isDone()) {
        throw std::logic_error("Cannot make a turn: coroutine has already finished.");
    }

    channel_->state = currentState;
    coro_.resume();

    if (coro_.promise().exception) {
        std::rethrow_exception(coro_.promise().exception);
    }

    if (!coro_.promise().lastResult.has_value()) {
        throw std::logic_error("Coroutine did not yield a turn result.");
    }

    return *coro_.promise().lastResult;
}

void PlayerCoroutine::finishWinner() {
    if (coro_ && !coro_.done()) {
        coro_.resume();
        if (coro_.promise().exception) {
            std::rethrow_exception(coro_.promise().exception);
        }
    }
}

PlayerCoroutine makePlayerCoroutine(
    PlayerId id,
    std::string name,
    std::shared_ptr<IStrategy> strategy,
    std::shared_ptr<GameStateChannel> channel
) {
    std::optional<ActionType> lastAction = std::nullopt;

    while (true) {
        GameState currentState = channel->state;

        Action action = strategy->chooseAction(currentState, lastAction);
        lastAction = action.type;

        currentState.applyAction(action);

        if (currentState.isWinning()) {
            channel->state = currentState;
            co_yield TurnResult{ action, currentState, true };

            std::cout << std::format(
                "\nWINNER ANNOUNCEMENT (ID: {}, {}):\n"
                "\"I won! Target sum reached.\"\n"
                "Numbers: a = {}, b = {}.\n"
                "Current sum = {}, which is >= 10x initial sum (10 * {} = {})!\n"
                "Winner coroutine terminates.\n\n",
                id, name, currentState.a(), currentState.b(),
                currentState.currentSum(), currentState.initialSum(), currentState.targetSum()
            );

            co_return;
        }

        channel->state = currentState;
        co_yield TurnResult{ action, currentState, false };
    }
}

}
