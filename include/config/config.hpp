#pragma once

#include <unordered_map>

#include "types.hpp"
#include "config/target.hpp"
#include "config/toolchain.hpp"

class Config{
    private:
        umap<string, Target> targets;
        umap<string, Toolchain> toolchains;

    public:
        Config() = delete;

        Config(stringview config_path);

        const umap<string, Target>& getTargets() const        {return targets;}
        const umap<string, Toolchain>& getToolchains() const  {return toolchains;}
};