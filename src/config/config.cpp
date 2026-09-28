#include "config/config.hpp"

#include <iostream>
#include <toml/toml.hpp>

//Add exepcion checked exception throwing instead of return to know when it fails
Config::Config(stringview config_path){
    
	try{
		toml::table tbl = toml::parse_file(config_path);

		//Read Targets
		auto targets_tbl = tbl.at("targets").as_table();
		if(!targets_tbl){
			std::cerr << "No targets found\n";
			return;
		}
		for(auto&& [name,value] : *targets_tbl)
		{
			auto target_tbl = value.as_table();
            this->targets.emplace(name, Target{name, *target_tbl});
		}

		//Build and check dependency graph before reading rest

		//Read toolchains
		auto toolchains_tbl = tbl.at("toolchain").as_table();
		if(!toolchains_tbl){
			std::cerr << "No toolchains found\n";
			return;
		}
		for (auto&& [name, value] : *toolchains_tbl){
			auto toolchain = value.as_table();
			this-> toolchains.emplace(name, Toolchain{name, *toolchain});
		}
		//Read modes
		auto modes_tbl = tbl.at("modes").as_table();
		if(!modes_tbl){
			std::cerr << "No build modes found";
			return;
		}
		for(auto&& [name, value] : *modes_tbl){
			auto mode = value.as_table();
			this->build_modes.emplace(name, Mode{name, *mode});
		}
	}
	catch (const toml::parse_error& err){
		std::cerr << "Parsing failed: \n" << err << "\n";
		return;
	}
}