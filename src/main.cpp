#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cstring>

using string = std::string;

const string configFileName{"autoc.conf"};

std::unordered_map<string, string> map_settings_file(std::filesystem::path& path);

int main() {
	
	auto cwd = std::filesystem::current_path();
	auto filePath {cwd / configFileName};
	
	auto configMap = map_settings_file(filePath);
	

	return 0;
}

std::unordered_map<string, string> map_settings_file(std::filesystem::path& path){
	std::ifstream configFile(path);

	if(!configFile){
		std::cerr << "Couldn't open configuration file\n";
		return {};
	}

	std::unordered_map<string, string> map;

	string line;
	while(std::getline(configFile, line)){
		char *token = std::strtok(line.data(), " =");
		if(!token) continue;
		string name {token};
		
		token = std::strtok(nullptr, " =");
		if(!token) continue;
		string value {token};

		map[name] = value;
	}

	/*for(const auto& [key, value] : map){
		std::cout << key << " " << value << "\n";
	}*/

	/*No hace falta pointer, c++ puede hacer esto de manera eficiente*/
	return map;
}
