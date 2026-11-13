#pragma once
#include "game_state.h"
#include <memory>
#include <random>
#include <string>

namespace Game {

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual std::string name() const = 0;
    virtual Action chooseAction(const GameState& state, std::optional<ActionType> lastAction) = 0;
};

class GreedyStrategy : public IStrategy {
public:
    std::string name() const override { return "Greedy"; }
    Action chooseAction(const GameState& state, std::optional<ActionType> lastAction) override;
};

class RandomStrategy : public IStrategy {
public:
    explicit RandomStrategy(std::optional<uint64_t> seed = std::nullopt);
    std::string name() const override { return "Random"; }
    Action chooseAction(const GameState& state, std::optional<ActionType> lastAction) override;

private:
    std::mt19937_64 rng_;
};

}
