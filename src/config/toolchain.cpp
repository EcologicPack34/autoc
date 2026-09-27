#include "toolchain.hpp"

#include <iostream>

Toolchain::Toolchain(stringview name, toml::table& table){
    auto get_string = [&](const string& key) -> optional<stringview> {
        auto val = table[key].value<stringview>();

        if (!val) {
            std::cerr << "[Target: " << name << "]: Missing '" << key << "'\n";
            return {};
        }
        return *val;
    };
}