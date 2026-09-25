#include <iostream>
#include <filesystem>

#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "file_utils.hpp"

#include "modules/c_cpp_module.hpp"

#include "config/config.hpp"

const string configFileName{"autoc.toml"};

int main() {
	
	auto cwd{ std::filesystem::current_path() };
	string filePath {cwd / configFileName};

	/*C_CPP_Module main_module{cwd};
	if(main_module.read_settings(map_settings_file(filePath)) == false){
		std::cerr << "Could not map settings correctly\n";
		return 1;
	}
	if(main_module.create_project_structure() == false){
		std::cerr << "Could not create project structure correctly\n";
		return 1;
	}
	main_module.compile();
	main_module.link();*/

	Config config{filePath};

	for(auto& [key, value]: config.getTargets()){
		std::cout << key << " " << value.getName() << "\n";
	}

	return 0;
}
