#pragma once

#include "types.hpp"

#include "toml/toml.hpp"

class Toolchain{
    private:
        string c_compiler;
        string cpp_compiler;

    public:
        Toolchain() = delete;
        Toolchain(stringview name, toml::table& table);

        stringview getCCompiler()   {return c_compiler;}
        stringview getCppCompiler() {return cpp_compiler;}
};
