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
		
		for(auto&& [name,value] : *targets_tbl){
			auto target_tbl = value.as_table();
			
			std::optional<Target> target = parse_from_table(name, *target_tbl);
			if(!target) return;

			//valid because string creates copy of stringview
			//less memory efficient, but safer because if not when doing move it would break
            this->targets.emplace((*target).getName(), std::move(*target));
		}
	}
	catch (const toml::parse_error& err){
		std::cerr << "Parsing failed: \n" << err << "\n";
		return;
	}
}