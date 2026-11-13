#pragma once
#include "types.h"
#include "game_state.h"
#include "player_coroutine.h"
#include "strategy.h"
#include <memory>
#include <string>

namespace Game {

struct GameConfig {
    Number a0 = 3;
    Number b0 = 5;
    std::string player1Name = "Coroutine-1 (Alpha)";
    std::string player2Name = "Coroutine-2 (Beta)";
    std::shared_ptr<IStrategy> strategy1;
    std::shared_ptr<IStrategy> strategy2;
    size_t maxTurns = 500;
};

class GameEngine {
public:
    explicit GameEngine(GameConfig config);

    PlayerId runGame();

private:
    void printHeader() const;
    void printTurn(size_t turnNum, PlayerId playerId, const std::string& playerName, const Action& action, const GameState& stateBefore, const GameState& stateAfter) const;

    GameConfig config_;
    GameState state_;
};

}
