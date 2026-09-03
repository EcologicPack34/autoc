#include "file_utils.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

#include "utils.hpp"

std::unordered_map<string, string> map_settings_file(const std::filesystem::path& path){
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

//local overload of the function for recursion
void get_files_in_dir(const std::filesystem::path& path, const std::vector<string>& extensions, std::vector<std::filesystem::path>& result){
    for(const auto& entry : std::filesystem::directory_iterator(path))
    {
		if(entry.is_directory())
        {
			get_files_in_dir(entry.path(), extensions, result);
		}
        else
        {	
			const auto& file_path = entry.path();
			const auto& extension = file_path.extension();

			if(extensions.empty() ||
				vector_contains<string>(extensions, extension.string()))
			{
				result.push_back(file_path);
			}
		}
	}
}

std::vector<std::filesystem::path> get_files_in_dir(const std::filesystem::path& path, const std::vector<string>& extensions){
    
    std::vector<std::filesystem::path> result;
    
    get_files_in_dir(path, extensions, result);

    return result;
}

