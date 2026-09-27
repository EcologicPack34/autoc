#pragma once

#include <vector>

#include "types.hpp"

#include "toml/toml.hpp"

#include <optional>

class Target{

    public:
        enum Type{
            UNKNOWN,
            EXECUTABLE,
            STATIC_LIB
        };

        static stringview typeToString(Type type);
        static Type typeFromString(stringview str);

    private:
        string name;
        Type type;

        string output_name;

        vec<string> compile_flags;
        vec<string> link_flags;

        vec<string> static_libraries;

        //other target dependencies
        vec<string> dependencies;
    public:
        Target() = delete;
        Target(stringview name, toml::table& target);

        stringview getName() const              {return name;}
        Type getType() const  {return type;}
        stringview getOutputName() const        {return output_name;}

        const vec<string>& getDependencies() const  {return dependencies;}

        const vec<string>& getCompileFlags() const  {return compile_flags;}
        const vec<string>& getLinkFlags() const     {return link_flags;}

        const vec<string>& getLibraries() const         {return static_libraries;}
    
};

//optional<Target> parse_from_table(stringview name, toml::table& target);