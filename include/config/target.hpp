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

        std::vector<string> source_directories;
        std::vector<string> include_directories;

        std::vector<string> compile_flags;
        std::vector<string> link_flags;

        std::vector<string> lib_directories;
        std::vector<string> static_libraries;

        //other target dependencies
        std::vector<string> dependencies;
    public:
        Target() = delete;
        Target(stringview name, Type type, stringview output_name,
                const std::vector<string>& source_directories, const std::vector<string>& include_directories, 
                const std::vector<string>& compile_flags, const std::vector<string>& link_flags,
                const std::vector<string>& lib_directories, const std::vector<string>& static_libraries,
                const std::vector<string>& dependencies);

        stringview getName() const              {return name;}
        Type getType() const  {return type;}
        stringview getOutputName() const        {return output_name;}

        const std::vector<string>& getSourceDirectories() const     {return source_directories;}
        const std::vector<string>& getIncludeDirectories() const    {return include_directories;}

        const std::vector<string>& getDependencies() const  {return dependencies;}

        const std::vector<string>& getCompileFlags() const  {return compile_flags;}
        const std::vector<string>& getLinkFlags() const     {return link_flags;}

        const std::vector<string>& getLibDirectories() const    {return lib_directories;}
        const std::vector<string>& getLibraries() const         {return static_libraries;}
    
};

std::optional<Target> parse_from_table(stringview name, toml::table& target);