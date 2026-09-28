#include "toolchain.hpp"

#include <iostream>

//Change return to exception throwing
Toolchain::Toolchain(stringview name, toml::table& table){
    auto get_string = [&](const string& key) -> optional<stringview> {
        auto val = table[key].value<stringview>();

        if (!val) {
            std::cerr << "[Target: " << name << "]: Missing '" << key << "'\n";
            return {};
        }
        return *val;
    };
    
    auto cc = get_string("cc");
    if(!cc) return;

    auto cpp = get_string("cxx");
    if(!cpp) return;

    this->name = name;
    this->c_compiler = *cc;
    this->cpp_compiler = *cpp;
}