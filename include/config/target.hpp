#pragma once

#include <vector>

#include "types.hpp"

#include "toml/toml.hpp"

#include <optional>

namespace TargetType{
    enum TargetType{
        UNKNOWN,
        EXECUTABLE,
        STATIC_LIB
    };

    inline stringview toString(TargetType type){
        switch (type)
        {
        case EXECUTABLE:
            return "executable";
        case STATIC_LIB:
            return "static_lib";
        default:
            return "unknown";
        }
    }

    inline TargetType fromString(stringview str){
        if (str == "executable")
            return TargetType::EXECUTABLE;

        if (str == "static_lib")
            return TargetType::STATIC_LIB;

        return TargetType::UNKNOWN;
    }
}

class Target{

    private:
        string name;
        TargetType::TargetType type;

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
        Target(stringview name, TargetType::TargetType type, stringview output_name,
                const std::vector<string>& source_directories, const std::vector<string>& include_directories, 
                const std::vector<string>& compile_flags, const std::vector<string>& link_flags,
                const std::vector<string>& lib_directories, const std::vector<string>& static_libraries,
                const std::vector<string>& dependencies)
        {
            this->name = name;
            this->type = type;
            this->output_name = output_name;

            this->source_directories  = source_directories;
            this->include_directories = include_directories;

            this->compile_flags = compile_flags;
            this->link_flags = link_flags;

            this->lib_directories = lib_directories;
            this->static_libraries = static_libraries;

            this->dependencies = dependencies;
        }

        stringview getName() const              {return name;}
        TargetType::TargetType getType() const  {return type;}
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