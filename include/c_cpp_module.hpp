#ifndef C_CPP_MODULE_H_
#define C_CPP_MODULE_H_

#include "types.hpp"

#include <vector>
#include <filesystem>
#include <unordered_map>

void compile(const std::filesystem::path& cwd, const std::unordered_map<string, string>& settings);

#endif