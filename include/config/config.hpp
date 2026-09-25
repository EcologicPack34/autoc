#pragma once

#include <unordered_map>

#include "types.hpp"
#include "config/target.hpp"

class Config{
    private:
        std::unordered_map<string, Target> targets;

    public:
        Config() = delete;

        Config(stringview config_path);

        const std::unordered_map<string, Target>& getTargets(){return targets;}
};