#pragma once
#include <coroutine>
#include <memory>
#include <string>
#include <optional>
#include <exception>
#include "types.h"
#include "action.h"
#include "game_state.h"
#include "strategy.h"

namespace Game {

struct TurnResult {
    Action action;
    GameState stateAfter;
    bool isWinner = false;
};

struct GameStateChannel {
    GameState state;
    explicit GameStateChannel(const GameState& s) : state(s) {}
};

class PlayerCoroutine {
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    explicit PlayerCoroutine(handle h, std::shared_ptr<GameStateChannel> channel)
        : coro_(h), channel_(channel) {}

    ~PlayerCoroutine() {
        if (coro_) {
            coro_.destroy();
        }
    }

    PlayerCoroutine(const PlayerCoroutine&) = delete;
    PlayerCoroutine& operator=(const PlayerCoroutine&) = delete;

    PlayerCoroutine(PlayerCoroutine&& other) noexcept
        : coro_(other.coro_), channel_(std::move(other.channel_)) {
        other.coro_ = nullptr;
    }

    PlayerCoroutine& operator=(PlayerCoroutine&& other) noexcept {
        if (this != &other) {
            if (coro_) {
                coro_.destroy();
            }
            coro_ = other.coro_;
            channel_ = std::move(other.channel_);
            other.coro_ = nullptr;
        }
        return *this;
    }

    bool isDone() const noexcept {
        return !coro_ || coro_.done();
    }

    TurnResult playTurn(const GameState& currentState);
    void finishWinner();

    struct promise_type {
        std::optional<TurnResult> lastResult{ std::nullopt };
        std::exception_ptr exception{nullptr};
        std::shared_ptr<GameStateChannel> channel;

        promise_type(PlayerId, const std::string&, std::shared_ptr<IStrategy>, std::shared_ptr<GameStateChannel> chan)
            : channel(chan) {}

        PlayerCoroutine get_return_object() {
            return PlayerCoroutine{ handle::from_promise(*this), channel };
        }

        auto initial_suspend() noexcept { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }

        auto yield_value(TurnResult res) noexcept {
            lastResult = res;
            return std::suspend_always{};
        }

        void return_void() noexcept {}

        void unhandled_exception() {
            exception = std::current_exception();
        }
    };

private:
    handle coro_;
    std::shared_ptr<GameStateChannel> channel_;
};

PlayerCoroutine makePlayerCoroutine(
    PlayerId id,
    std::string name,
    std::shared_ptr<IStrategy> strategy,
    std::shared_ptr<GameStateChannel> channel
);

}
