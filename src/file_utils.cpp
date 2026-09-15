#include "file_utils.hpp"

#include <fstream>
#include <iostream>
#include <cstring>


#include "utils.hpp"

std::unordered_map<string, string> map_settings_file(const std::filesystem::path& path){
	
	std::ifstream configFile(path);

    if (!configFile) {
        std::cerr << "Couldn't open configuration file\n";
        return {};
    }

    std::unordered_map<std::string, std::string> map;

    string line;
    while (std::getline(configFile, line)) {
        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        string name = line.substr(0, pos);
        string value = line.substr(pos + 1);

        // Trim whitespace from the key
        auto first = name.find_first_not_of(" \t");
        auto last  = name.find_last_not_of(" \t");

        if (first == string::npos)
            continue;

        name = name.substr(first, last - first + 1);

        // Trim whitespace from the value
        first = value.find_first_not_of(" \t");
        last  = value.find_last_not_of(" \t");

        if (first != string::npos)
            value = value.substr(first, last - first + 1);
        else
            value.clear();

        map[name] = value;

		//std::cout << name << " " << value << "\n";
    }

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

std::vector<string> string_to_vector(const string& str, char separator){

	std::stringstream stream{str};
	string token;

	std::vector<string> output;
	while(std::getline(stream, token, separator)){
		output.push_back(token);
	}

	return output;
}

std::optional<int> compare_file_mod_dates(const std::filesystem::path& file1, const std::filesystem::path& file2){

    if(std::filesystem::exists(file1) == false || std::filesystem::exists(file2) == false){
        return {};
    }

    auto file1_time = std::filesystem::last_write_time(file1);
    auto file2_time = std::filesystem::last_write_time(file2);

    if(file1_time < file2_time) return -1;
    else if(file1_time > file2_time) return 1;
    else return 0;
}