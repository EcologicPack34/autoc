#include "config/target.hpp"

#include <optional>
#include <iostream>


stringview Target::typeToString(Target::Type type){
    
    switch (type)
        {
        case Target::EXECUTABLE:
            return "executable";
        case Target::STATIC_LIB:
            return "static_lib";
        default:
            return "unknown";
        }
}

Target::Type Target::typeFromString(stringview str){
    if (str == "executable")
            return Target::EXECUTABLE;

        if (str == "static_lib")
            return Target::STATIC_LIB;

        return Target::UNKNOWN;
}

std::optional<Target> parse_from_table(stringview name, toml::table& target){
    auto get_string = [&](const string& key) -> std::optional<stringview> {
        auto it = target[key].value<stringview>();

        if (!it) {
            std::cerr << "[Target: " << name << "]: Missing '" << key << "'\n";
            return {};
        }
        return *it;
    };

    auto get_vector = [&](const std::string& key) -> std::optional<std::vector<string>>
    {
        auto arr = target[key].as_array();

        if (!arr) {
            std::cerr << "[Target: " << name << "]: Missing '" << key << "'\n";
            return {};
        }

        std::vector<std::string> result;

        for (const auto& item : *arr) {
            auto value = item.value<string>();

            if (!value) {
                std::cerr << "[Target: " << name << "]: Invalid value in '" << key << "'\n";
                return {};
            }
            result.push_back(std::move(*value));
        }
        return result;
    };
    
    auto type = get_string("type");
    if (!type) return {};
    auto typeEnum = Target::typeFromString(*type);
    if(typeEnum == Target::UNKNOWN){
        std::cerr << "[Target: " << name << "]: Unkown target type '" << *type << "'\n";
        return {};
    } 

    auto output = get_string("output");
    if (!output) return {};

    auto source_directories = get_vector("source_directories");
    if(!source_directories) return {};
    if((*source_directories).size() < 1){
        std::cerr << "[Target: " << name << "]: source_directories field can't be empty\n";
        return {};
    }

    auto include_directories = get_vector("include_directories");
    if(!source_directories) return {};

    auto deps = get_vector("dependencies");
    if(!source_directories) return {};

    auto compile_flags = get_vector("compile_flags");
    if(!compile_flags) return {};

    auto link_flags = get_vector("link_flags");
    if(!link_flags) return {};

    auto lib_directories = get_vector("lib_directories");
    if(!lib_directories) return {};

    auto static_libraries = get_vector("static_libraries");
    if(!static_libraries) return {};

    return Target{name, typeEnum, *output,
         *source_directories, *include_directories, 
         *compile_flags, *link_flags, 
         *lib_directories, *static_libraries, *deps};
}