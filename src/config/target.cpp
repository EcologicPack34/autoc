#include "config/target.hpp"

#include <optional>
#include <iostream>

std::optional<Target> parse_from_table(stringview name, toml::table& target){
    auto get_string = [&](const string& key) -> stringview* {
        auto it = target[key].value<stringview>();

        if (!it) {
            std::cerr << "[Target: " << name << "]: Missing '" << key << "'\n";
            return nullptr;
        }
        return &(*it);
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
    auto typeEnum = TargetType::fromString(*type);
    if(typeEnum == TargetType::UNKNOWN){
        std::cerr << "[Target: " << name << "]: Unkown target type '" << *type << "'\n";
        return {};
    } 

    auto output = get_string("output");
    if (!output) return {};

    auto source_directories = get_vector("source_directories");
    if(!source_directories) return {};

    auto include_directories = get_vector("include_directories");
    if(!source_directories) return {};

    auto deps = get_vector("dependencies");
    if(!source_directories) return {};

    return Target{name, typeEnum, *output, *source_directories, *include_directories, *deps};
}