#pragma once

#include "types.hpp"

#include <toml/toml.hpp>

class Mode{
    private:
        string name;
        vec<string> compile_flags;
        vec<string> link_flags;
        vec<string> defines;
    public:
        Mode() = delete;
        Mode(stringview name, toml::table& table);

        stringview getName() const                  {return name;}
        const vec<string>& getCompileFlags() const  {return compile_flags;}
        const vec<string>& getLinkFlags() const     {return link_flags;}
        const vec<string>& getDefines() const       {return defines;}
};