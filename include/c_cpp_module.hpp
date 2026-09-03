#ifndef C_CPP_MODULE_H_
#define C_CPP_MODULE_H_

#include "types.hpp"

#include <vector>
#include <filesystem>

void compile(const std::vector<std::filesystem::path>& files, const std::filesystem::path& cwd, const std::vector<string>& compile_flags);

#endif