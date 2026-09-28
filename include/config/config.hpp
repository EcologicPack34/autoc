#pragma once

#include <unordered_map>

#include "types.hpp"
#include "config/target.hpp"
#include "config/toolchain.hpp"
#include "config/mode.hpp"


//Change so that Target, Toolchain, Mode, etc. inherit from common class?
class Config{
    private:
        umap<string, Target> targets;
        umap<string, Toolchain> toolchains;
        umap<string, Mode> build_modes;

    public:
        Config() = delete;

        Config(stringview config_path);

        const umap<string, Target>& getTargets() const          {return targets;}
        const umap<string, Toolchain>& getToolchains() const    {return toolchains;}
        const umap<string, Mode>& getBuildModes() const         {return build_modes;}
};