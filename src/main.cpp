#include <iostream>
#include <filesystem>

#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "file_utils.hpp"

#include "c_cpp_module.hpp"

const string configFileName{"autoc.conf"};


void file_iterate(const std::filesystem::path& path);

int main() {
	
	auto cwd{ std::filesystem::current_path() };
	auto filePath {cwd / configFileName};

	if(compile(cwd, map_settings_file(filePath)) == false){
		return 1;
	}

	return 0;
}
