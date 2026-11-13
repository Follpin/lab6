#include <cassert>
#include <iostream>
#include <vector>
#include <memory>
#include <format>
#include "types.h"
#include "action.h"
#include "game_state.h"
#include "strategy.h"
#include "input_validator.h"
#include "player_coroutine.h"
#include "game_engine.h"

using namespace Game;

void testInputValidator() {
    std::cout << "[RUN] testInputValidator...\n";
    Number val = 0;

    auto [ok1, err1] = InputValidator::validateAndParseNumber("42", val);
    assert(ok1 && val == 42);

    auto [ok2, err2] = InputValidator::validateAndParseNumber("   +10   ", val);
    assert(ok2 && val == 10);

    auto [ok3, err3] = InputValidator::validateAndParseNumber("1", val);
    assert(ok3 && val == 1);

    auto [okZero, _] = InputValidator::validateAndParseNumber("0", val);
    assert(!okZero);

    auto [okNeg, _2] = InputValidator::validateAndParseNumber("-5", val);
    assert(!okNeg);

    auto [okFloat, _3] = InputValidator::validateAndParseNumber("3.14", val);
    assert(!okFloat);

    auto [okAlpha, _4] = InputValidator::validateAndParseNumber("abc", val);
    assert(!okAlpha);

    auto [okEmpty, _5] = InputValidator::validateAndParseNumber("", val);
    assert(!okEmpty);

    std::cout << "[PASS] testInputValidator\n";
}

void testGameStateAndActions() {
    std::cout << "[RUN] testGameStateAndActions...\n";
    GameState state(3, 6);
    assert(state.a() == 3);
    assert(state.b() == 6);
    assert(state.initialSum() == 9);
    assert(state.currentSum() == 9);
    assert(state.targetSum() == 90);
    assert(!state.isWinning());

    state.applyAction(Action{ ActionType::AddOne, TargetNumber::A });
    assert(state.a() == 4);
    assert(state.currentSum() == 10);

    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::B });
    assert(state.b() == 12);
    assert(state.currentSum() == 16);

    state.applyAction(Action{ ActionType::DivideThree, TargetNumber::B });
    assert(state.b() == 4);
    assert(state.currentSum() == 8);

    bool thrown = false;
    try {
        state.applyAction(Action{ ActionType::DivideThree, TargetNumber::A });
    } catch (const std::logic_error&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] testGameStateAndActions\n";
}

void testActionValidityRules() {
    std::cout << "[RUN] testActionValidityRules...\n";
    GameState state(4, 9);

    assert(state.isActionValid(Action{ ActionType::AddOne, TargetNumber::A }, std::nullopt));
    assert(state.isActionValid(Action{ ActionType::MultiplyTwo, TargetNumber::A }, std::nullopt));
    assert(!state.isActionValid(Action{ ActionType::DivideThree, TargetNumber::A }, std::nullopt));
    assert(state.isActionValid(Action{ ActionType::DivideThree, TargetNumber::B }, std::nullopt));

    std::optional<ActionType> last = ActionType::MultiplyTwo;
    assert(!state.isActionValid(Action{ ActionType::MultiplyTwo, TargetNumber::A }, last));
    assert(!state.isActionValid(Action{ ActionType::MultiplyTwo, TargetNumber::B }, last));
    assert(state.isActionValid(Action{ ActionType::AddOne, TargetNumber::A }, last));
    assert(state.isActionValid(Action{ ActionType::DivideThree, TargetNumber::B }, last));

    auto valid = state.getValidActions(ActionType::AddOne);
    for (const auto& act : valid) {
        assert(act.type != ActionType::AddOne);
        if (act.type == ActionType::DivideThree) {
            assert(act.target == TargetNumber::B);
        }
    }

    std::cout << "[PASS] testActionValidityRules\n";
}

void testWinningCondition() {
    std::cout << "[RUN] testWinningCondition...\n";
    GameState state(5, 5);
    assert(!state.isWinning());

    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::A });
    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::A });
    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::A });
    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::A });
    state.applyAction(Action{ ActionType::MultiplyTwo, TargetNumber::A });
    assert(state.isWinning());

    std::cout << "[PASS] testWinningCondition\n";
}

void testPlayerCoroutine() {
    std::cout << "[RUN] testPlayerCoroutine...\n";
    GameState state(10, 10);
    auto channel = std::make_shared<GameStateChannel>(state);
    auto strategy = std::make_shared<GreedyStrategy>();

    PlayerCoroutine player = makePlayerCoroutine(1, "Test Player", strategy, channel);
    assert(!player.isDone());

    TurnResult res1 = player.playTurn(state);
    assert(res1.action.type == ActionType::MultiplyTwo);
    assert(!res1.isWinner);
    assert(!player.isDone());

    TurnResult res2 = player.playTurn(res1.stateAfter);
    assert(res2.action.type != ActionType::MultiplyTwo);
    assert(!player.isDone());

    std::cout << "[PASS] testPlayerCoroutine\n";
}

void testFullGameSimulation() {
    std::cout << "[RUN] testFullGameSimulation...\n";
    GameConfig config;
    config.a0 = 2;
    config.b0 = 4;
    config.strategy1 = std::make_shared<GreedyStrategy>();
    config.strategy2 = std::make_shared<RandomStrategy>(12345);

    GameEngine engine(config);
    PlayerId winner = engine.runGame();

    assert(winner == 1 || winner == 2);
    std::cout << "[PASS] testFullGameSimulation (Winner: " << winner << ")\n";
}

int main() {
    std::cout << "Running test suite (Lab 6 - Variant 12)...\n\n";

    testInputValidator();
    testGameStateAndActions();
    testActionValidityRules();
    testWinningCondition();
    testPlayerCoroutine();
    testFullGameSimulation();

    std::cout << "\nALL TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
