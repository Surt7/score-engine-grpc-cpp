
function on_switch(switch_code)
    if switch_code == "BUMPER_1" then
        return { score_delta = 100, action = "BumperHit" }
    elseif switch_code == "SUPER_BUMPER" then
        return { score_delta = 500, action = "BumperHit" }
    end

    return nil
end