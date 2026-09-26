#pragma once

#include "types.hpp"

class Toolchain{
    private:
        string c_compiler;
        string cpp_compiler;

    public:
        Toolchain() = delete;
        Toolchain(stringview c_compiler, stringview cpp_compiler)
            : c_compiler{c_compiler}, cpp_compiler{cpp_compiler} {};

        stringview getCCompiler()   {return c_compiler;}
        stringview getCppCompiler() {return cpp_compiler;}
};

//optional<Toolchain> parse_from_table(stringview name, toml::table& target);
