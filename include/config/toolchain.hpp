#pragma once

#include "types.hpp"

#include "toml/toml.hpp"

class Toolchain{
    private:
        string name;
        string c_compiler;
        string cpp_compiler;

    public:
        Toolchain() = delete;
        Toolchain(stringview name, toml::table& table);

        stringview getName() const          {return name;}
        stringview getCCompiler() const     {return c_compiler;}
        stringview getCppCompiler() const   {return cpp_compiler;}
};
