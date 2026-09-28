#include "config/mode.hpp"

#include <iostream>

Mode::Mode(stringview name, toml::table& table){
    auto get_vector = [&](const std::string& key) -> optional<vec<string>>
    {
        auto arr = table.at(key).as_array();

        if (!arr) {
            std::cerr << "[Mode: " << name << "]: Missing '" << key << "'\n";
            return {};
        }

        vec<std::string> result;

        for (const auto& item : *arr) {
            auto value = item.value<string>();

            if (!value) {
                std::cerr << "[Mode: " << name << "]: Invalid value in '" << key << "'\n";
                return {};
            }
            result.push_back(std::move(*value));
        }
        return result;
    };

    auto comp_flags = get_vector("compile_flags");
    if(!comp_flags) return;

    auto link_flags = get_vector("link_flags");
    if(!link_flags) return;

    auto defines = get_vector("defines");
    if(!defines) return;

    this->name = name;
    this->compile_flags = *comp_flags;
    this->link_flags = *link_flags;
    this->defines = *defines;
}