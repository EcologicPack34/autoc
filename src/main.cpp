#include <iostream>
#include <filesystem>

#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "file_utils.hpp"


const string configFileName{"autoc.conf"};


void file_iterate(const std::filesystem::path& path);

int main() {
	
	auto cwd = std::filesystem::current_path();
	auto filePath {cwd / configFileName};
	
	auto configMap = map_settings_file(filePath);

	//file_iterate(cwd);

	for(const auto& e : get_files_in_dir(cwd, {".cpp", ".hpp"})){
		std::cout << e << "\n";
	}

	return 0;
}



void file_iterate(const std::filesystem::path& path){
	for(const auto& entry : std::filesystem::directory_iterator(path)){
		if(entry.is_directory()){
			file_iterate(entry.path());
		}else{
			std::cout << entry.path() << "\n";
		}
	}
}
