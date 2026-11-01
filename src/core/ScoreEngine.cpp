#include "../../include/core/ScoreEngine.h"

EngineResult ScoreEngine::process_event(const Event &event) {
    // check anti replay
    if (event.sequence_id <= last_sequence_id_) {
        return EngineResult{
            .status = EngineStatus::RejectedReplay,
            .sequence_id = event.sequence_id,
            .score_delta = 0,
            .total_score = total_score_.value(),
            .action = GameAction::RejectedReplay
        };
    }

    // check signature
    if (event.signature != "VALID_SECRET") {
        return EngineResult{
            .status = EngineStatus::RejectedInvalidSignature,
            .action = GameAction::RejectedInvalidSignature
        };
    }

    // update the sequence_id
    last_sequence_id_ = event.sequence_id;

    int32_t score_delta{0};
    auto action{GameAction::None};

    bool rule_applied{false};

    for (const auto &rule: rules_) {
        if (rule->matches(event)) {
            const auto effect = rule->apply(event);
            score_delta = effect.score_delta;
            action = effect.action;
            rule_applied = true;
            break;
        }
    }


    if (!rule_applied) {
        return EngineResult{
            .status = EngineStatus::RejectedUnknownSwitch,
            .sequence_id = event.sequence_id,
            .score_delta = 0,
            .total_score = total_score_.value(),
            .action = GameAction::UnknownSwitch
        };
    }


    if (action == GameAction::GameStart) {
        state_ = GameState::InGame;
        total_score_.reset();
        current_ball_ = 1;
    } else if (state_ == GameState::InGame) {
        if (action == GameAction::BallDrained) {
            if (current_ball_ < MAX_BALLS) {
                ++current_ball_;
            } else {
                state_ = GameState::GameOver;
                action = GameAction::GameOver;
            }
        } else {
            total_score_ += score_delta;
        }
    }

    return EngineResult{
        .status = EngineStatus::Accepted,
        .sequence_id = event.sequence_id,
        .score_delta = score_delta,
        .total_score = total_score_,
        .action = action,
        .state = state_
    };
}


void ScoreEngine::add_rule(std::unique_ptr<IRule> rule) {
    rules_.push_back(std::move(rule));
}
