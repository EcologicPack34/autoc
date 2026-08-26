#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <unordered_map>
#include <filesystem>
#include <vector>

#include "types.hpp"

std::unordered_map<string, string> map_settings_file(const std::filesystem::path& path);

std::vector<string> get_files_in_dir(const std::filesystem::path& path, const std::vector<string>& extensions);

#endif