#pragma once
#include <filesystem>

#include "IRule.h"
#include <sol/sol.hpp>

class BumperRule : public IRule {
public:
    [[nodiscard]] bool matches(const Event &event) const override {
        return event.switch_code == "BUMPER_1";
    };

    RuleEffect apply(const Event &) override {
        return RuleEffect{
            .score_delta = 100,
            .action = GameAction::BumperHit
        };
    };
};

class StartButtonRule : public IRule {
public:
    [[nodiscard]] bool matches(const Event &event) const override {
        return event.switch_code == "START_BUTTON";
    }

    RuleEffect apply(const Event &) override {
        return RuleEffect{
            .score_delta = 0,
            .action = GameAction::GameStart
        };
    };
};

class DrainRule : public IRule {
public:
    [[nodiscard]] bool matches(const Event &event) const override {
        return event.switch_code == "DRAIN";
    }

    RuleEffect apply(const Event &) override {
        return RuleEffect{
            .score_delta = 0,
            .action = GameAction::BallDrained
        };
    }
};

class LuaRule : public IRule {
public:
    explicit LuaRule(const std::string &script_name) {
        lua_.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string);
        // Recherche automatique du bon chemin
        std::string resolved_path = script_name;
        for (const auto &candidate: {
                 script_name, "../../" + script_name, "../../../" + script_name,
                 "D:/Workspace/ScoreEngine/" + script_name
             }) {
            if (std::filesystem::exists(candidate)) {
                resolved_path = candidate;
                break;
            }
        }
        auto result = lua_.safe_script_file(resolved_path, sol::script_pass_on_error);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "[ERREUR Lua] Impossible de charger " << resolved_path << " : " << err.what() << "\n";
        } else {
            std::cout << "[Lua] Script charge avec succes depuis : " << resolved_path << "\n";
        }
    }

    [[nodiscard]] bool matches(const Event &event) const override {
        sol::function on_switch = lua_["on_switch"];
        if (!on_switch.valid()) return false;
        sol::object result = on_switch(event.switch_code);
        return result.is<sol::table>(); // La règle matche si Lua renvoie une table !
    }

    RuleEffect apply(const Event &event) override {
        sol::table res = lua_["on_switch"](event.switch_code);
        return RuleEffect{
            .score_delta = res.get_or("score_delta", 0),
            .action = GameAction::BumperHit
        };
    }

private:
    mutable sol::state lua_;
};
