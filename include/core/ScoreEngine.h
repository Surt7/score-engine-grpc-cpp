#pragma once
#include "Event.h"
#include "../rules/IRule.h"
#include <vector>
#include <memory>
#include "Score.h"

class ScoreEngine {
public:
    ScoreEngine() = default;

    EngineResult process_event(const Event &event);

    ~ScoreEngine() = default;

    void add_rule(std::unique_ptr<IRule> rule);

    [[nodiscard]] int64_t total_score() const { return total_score_.value(); }

    [[nodiscard]] GameState state() const { return state_; }

    [[nodiscard]] uint8_t current_ball() const { return current_ball_; }

    template<typename T, typename... Args>
    void add_rule(Args &&... args) {
        rules_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:
    Score total_score_;
    uint64_t last_sequence_id_{0};
    std::vector<std::unique_ptr<IRule> > rules_;
    GameState state_{GameState::AttractMode};
    uint8_t current_ball_{0};
    static constexpr uint8_t MAX_BALLS{3};
};
