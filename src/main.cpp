#include <iostream>
#include <filesystem>

#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "file_utils.hpp"

#include "modules/c_cpp_module.hpp"

#include <toml/toml.hpp>
#include "config/target.hpp"

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


	toml::table tbl;
	try{
		tbl = toml::parse_file(filePath);
		//std::cout << tbl << std::endl;

		auto targets_tbl = tbl["targets"].as_table();
		if(!targets_tbl){
			std::cerr << "No targets found\n";
			return 1;
		}
		
		
		std::vector<Target> targets;
		for(auto&& [name,value] : *targets_tbl){
			auto target_tbl = value.as_table();
			
			std::optional<Target> target = parse_from_table(name, *target_tbl);
			if(!target) return 1;

			targets.push_back(std::move(*target));
		}



	}
	catch (const toml::parse_error& err){
		std::cerr << "Parsing failed: \n" << err << "\n";
		return 1;
	}


	return 0;
}
