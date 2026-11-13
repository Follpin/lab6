#include <iostream>
#include <string>
#include <vector>
#include <format>
#include "types.h"
#include "input_validator.h"
#include "game_engine.h"
#include "strategy.h"

#ifdef _WIN32
#include <windows.h>
#endif

void configureConsoleUtf8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void printUsage(const char* progName) {
    std::cout << std::format("Usage: {} [a0] [b0] [strategy_mode]\n", progName);
    std::cout << "Parameters:\n";
    std::cout << "  a0, b0          - initial positive integers (> 0)\n";
    std::cout << "  strategy_mode   - optional game mode (1, 2, or 3):\n";
    std::cout << "                    1: Coroutine-1 (Greedy) vs Coroutine-2 (Random) [default]\n";
    std::cout << "                    2: Coroutine-1 (Greedy) vs Coroutine-2 (Greedy)\n";
    std::cout << "                    3: Coroutine-1 (Random) vs Coroutine-2 (Random)\n";
    std::cout << "\nExamples:\n";
    std::cout << std::format("  {} 3 5\n", progName);
    std::cout << std::format("  {} 7 12 2\n", progName);
    std::cout << std::format("  {} (interactive mode)\n", progName);
}

int main(int argc, char* argv[]) {
    configureConsoleUtf8();

    Game::Number a0 = 3;
    Game::Number b0 = 5;
    int strategyMode = 1;

    if (argc > 1) {
        std::string firstArg = argv[1];
        if (firstArg == "--help" || firstArg == "-h") {
            printUsage(argv[0]);
            return 0;
        }

        if (argc < 3) {
            std::cerr << "ERROR: At least two numbers (a0 and b0) must be provided.\n\n";
            printUsage(argv[0]);
            return 1;
        }

        auto [okA, errA] = Game::InputValidator::validateAndParseNumber(argv[1], a0);
        if (!okA) {
            std::cerr << std::format("ERROR for argument a0: {}\n", errA);
            return 1;
        }

        auto [okB, errB] = Game::InputValidator::validateAndParseNumber(argv[2], b0);
        if (!okB) {
            std::cerr << std::format("ERROR for argument b0: {}\n", errB);
            return 1;
        }

        if (argc >= 4) {
            Game::Number modeNum = 1;
            auto [okMode, errMode] = Game::InputValidator::validateAndParseNumber(argv[3], modeNum);
            if (okMode && (modeNum >= 1 && modeNum <= 3)) {
                strategyMode = static_cast<int>(modeNum);
            } else {
                std::cerr << "WARNING: Invalid strategy mode (expected 1, 2, or 3). Using mode 1.\n";
            }
        }
    } else {
        std::cout << "Lab 6. V12: Game of Two Coroutines \n\n";
        std::cout << "Enter initial numbers for the game (or press Enter for default values):\n";
        a0 = Game::InputValidator::promptNumber("Enter number a", 3);
        b0 = Game::InputValidator::promptNumber("Enter number b", 5);

        std::cout << "\nSelect coroutine strategy mode:\n";
        std::cout << "  1: Coroutine-1 (Greedy) vs Coroutine-2 (Random)\n";
        std::cout << "  2: Coroutine-1 (Greedy) vs Coroutine-2 (Greedy)\n";
        std::cout << "  3: Coroutine-1 (Random) vs Coroutine-2 (Random)\n";
        strategyMode = static_cast<int>(Game::InputValidator::promptNumber("Choose mode (1-3)", 1));
        if (strategyMode < 1 || strategyMode > 3) {
            strategyMode = 1;
        }
    }

    Game::GameConfig config;
    config.a0 = a0;
    config.b0 = b0;

    switch (strategyMode) {
        case 2:
            config.strategy1 = std::make_shared<Game::GreedyStrategy>();
            config.strategy2 = std::make_shared<Game::GreedyStrategy>();
            config.player1Name = "Coroutine-1 (Greedy)";
            config.player2Name = "Coroutine-2 (Greedy)";
            break;
        case 3:
            config.strategy1 = std::make_shared<Game::RandomStrategy>();
            config.strategy2 = std::make_shared<Game::RandomStrategy>();
            config.player1Name = "Coroutine-1 (Random)";
            config.player2Name = "Coroutine-2 (Random)";
            break;
        case 1:
        default:
            config.strategy1 = std::make_shared<Game::GreedyStrategy>();
            config.strategy2 = std::make_shared<Game::RandomStrategy>();
            config.player1Name = "Coroutine-1 (Greedy)";
            config.player2Name = "Coroutine-2 (Random)";
            break;
    }

    try {
        Game::GameEngine engine(config);
        engine.runGame();
    } catch (const std::exception& ex) {
        std::cerr << "FATAL ERROR: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
