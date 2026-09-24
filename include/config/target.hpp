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

        std::vector<string> dependencies;
    public:
        Target() = delete;
        Target(stringview name, TargetType::TargetType type, stringview output_name, const std::vector<string>& source_directories,
                const std::vector<string>& include_directories, const std::vector<string>& dependencies)
        {
            this->name = name;
            this->type = type;
            this->output_name = output_name;

            this->source_directories  = source_directories;
            this->include_directories = include_directories;
            this->dependencies = dependencies;
        }

        stringview getName(){return name;}
        TargetType::TargetType getType(){return type;}
        stringview getOutputName(){return output_name;}

        const std::vector<string>& getSourceDirectories(){return source_directories;}
        const std::vector<string>& getIncludeDirectories(){return include_directories;}
        const std::vector<string>& getDependencies(){return dependencies;}
    
};

std::optional<Target> parse_from_table(stringview name, toml::table& target);