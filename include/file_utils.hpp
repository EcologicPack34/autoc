#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <filesystem>
#include <vector>
#include <optional>

#include "types.hpp"

std::unordered_map<string, string> map_settings_file(const std::filesystem::path& path);

std::vector<std::filesystem::path> get_files_in_dir(const std::filesystem::path& path, const std::vector<string>& extensions);

std::vector<string> string_to_vector(const string& str, char separator = ' ');

/**
 * Returns -1 if file1 older, 0 if equal, 1 if file2 older
 */
std::optional<int> compare_file_mod_dates(const std::filesystem::path& file1, const std::filesystem::path& file2);

#endif