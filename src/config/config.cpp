#include "config/config.hpp"

#include <iostream>
#include <toml/toml.hpp>

//Add exepcion checked exception throwing instead of return to know when it fails
Config::Config(stringview config_path){
    
	try{
		toml::table tbl = toml::parse_file(config_path);

		auto targets_tbl = tbl["targets"].as_table();
		if(!targets_tbl){
			std::cerr << "No targets found\n";
			return;
		}
		
		for(auto&& [name,value] : *targets_tbl)
		{
			auto target_tbl = value.as_table();
            this->targets.emplace(name, std::move(Target{name, *target_tbl}));
		}
	}
	catch (const toml::parse_error& err){
		std::cerr << "Parsing failed: \n" << err << "\n";
		return;
	}
}