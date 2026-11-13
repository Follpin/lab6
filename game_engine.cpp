#include "game_engine.h"
#include <iostream>
#include <format>

namespace Game {

GameEngine::GameEngine(GameConfig config)
    : config_(std::move(config)), state_(config_.a0, config_.b0) {
    if (!config_.strategy1) {
        config_.strategy1 = std::make_shared<GreedyStrategy>();
    }
    if (!config_.strategy2) {
        config_.strategy2 = std::make_shared<RandomStrategy>(42);
    }
}

void GameEngine::printHeader() const {
    std::cout << "\nGAME PROTOCOL BETWEEN TWO COROUTINES (VARIANT 12)\n";
    std::cout << std::format("Initial numbers:           a0 = {}, b0 = {}\n", state_.a(), state_.b());
    std::cout << std::format("Initial sum S0:            {}\n", state_.initialSum());
    std::cout << std::format("Target winning sum (10x):  S >= {}\n", state_.targetSum());
    std::cout << std::format("Player 1:                  {} [Strategy: {}]\n", config_.player1Name, config_.strategy1->name());
    std::cout << std::format("Player 2:                  {} [Strategy: {}]\n", config_.player2Name, config_.strategy2->name());
    std::cout << std::format("{:<5} | {:<22} | {:<16} | {:<18} | {:<12}\n", "Turn", "Player", "Action", "State (a, b)", "Sum (cur/target)");
}

void GameEngine::printTurn(
    size_t turnNum,
    PlayerId playerId,
    const std::string& playerName,
    const Action& action,
    const GameState& stateBefore,
    const GameState& stateAfter
) const {
    (void)playerId;
    (void)stateBefore;
    std::string stateStr = std::format("({}, {})", stateAfter.a(), stateAfter.b());
    std::string sumStr = std::format("{}/{}", stateAfter.currentSum(), stateAfter.targetSum());

    std::cout << std::format(
        "{:<5} | {:<22} | {:<16} | {:<18} | {:<12}\n",
        turnNum,
        playerName,
        action.toString(),
        stateStr,
        sumStr
    );
}

PlayerId GameEngine::runGame() {
    printHeader();

    auto channel = std::make_shared<GameStateChannel>(state_);

    PlayerCoroutine player1 = makePlayerCoroutine(1, config_.player1Name, config_.strategy1, channel);
    PlayerCoroutine player2 = makePlayerCoroutine(2, config_.player2Name, config_.strategy2, channel);

    size_t turn = 1;
    PlayerId winner = 0;

    while (turn <= config_.maxTurns) {
        GameState beforeTurn1 = state_;
        TurnResult res1 = player1.playTurn(state_);
        state_ = res1.stateAfter;
        printTurn(turn, 1, config_.player1Name, res1.action, beforeTurn1, state_);

        if (res1.isWinner || state_.isWinning()) {
            winner = 1;
            player1.finishWinner();
            break;
        }

        GameState beforeTurn2 = state_;
        TurnResult res2 = player2.playTurn(state_);
        state_ = res2.stateAfter;
        printTurn(turn + 1, 2, config_.player2Name, res2.action, beforeTurn2, state_);

        if (res2.isWinner || state_.isWinning()) {
            winner = 2;
            player2.finishWinner();
            break;
        }

        turn += 2;
    }

    if (winner != 0) {
        std::string winnerName = (winner == 1) ? config_.player1Name : config_.player2Name;
        std::cout << std::format("\nSUMMARY: {} won in {} turns!\n", winnerName, (winner == 1 ? turn : turn + 1));
        std::cout << std::format("Final numbers: a = {}, b = {}, final sum = {} (target: >= {})\n\n",
            state_.a(), state_.b(), state_.currentSum(), state_.targetSum());
    } else {
        std::cout << "\nSUMMARY: Game ended due to turn limit without a winner.\n\n";
    }

    return winner;
}

}
