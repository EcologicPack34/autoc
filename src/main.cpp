#include <iostream>
#include <filesystem>

#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "file_utils.hpp"

#include "modules/c_cpp_module.hpp"

const string configFileName{"autoc.conf"};

int main() {
	
	auto cwd{ std::filesystem::current_path() };
	auto filePath {cwd / configFileName};

	C_CPP_Module main_module{cwd};
	if(main_module.read_settings(map_settings_file(filePath)) == false){
		std::cerr << "Could not map settings correctly\n";
		return 1;
	}
	if(main_module.create_project_structure() == false){
		std::cerr << "Could not create project structure correctly\n";
		return 1;
	}
	main_module.compile();
	main_module.link();

	return 0;
}
