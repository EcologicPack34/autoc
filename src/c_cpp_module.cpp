#include "c_cpp_module.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <unistd.h>
#include <sys/wait.h>

#include "file_utils.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

static const string src_dir{"src"};
static const string obj_dir{"obj"};
static const string include_dir{"include"};

static const string setting_compiler{"compiler"};
static const string setting_comp_flags{"compile_flags"};
static const string setting_src_extension{"src_extension"};

/*-----DEFINITIONS------*/

struct settings{
    std::string_view compiler;
    std::vector<string> compile_flags;
    std::vector<string> src_extensions;
};

bool read_settings(struct settings& settings, const std::unordered_map<string, string>& settings_map);

std::vector<fs::path> read_dependencies(const fs::path& dep_file);
std::vector<fs::path> get_include_dirs(const fs::path& cwd);

bool compile_file(const std::vector<string>& base_cmd, const fs::path& src, const fs::path& obj);
bool needs_recompilation(const fs::path& obj_file, const fs::path& dep_file);

void link_executable(const std::vector<fs::path>& obj_files, const struct settings& settings);

/*-----------IMPLEMENTATION------------*/

bool compile(const std::filesystem::path& cwd, const std::unordered_map<string, string>& settings_map){

    fs::path obj_path { cwd / obj_dir };
    if(!fs::exists(obj_path) || !fs::is_directory(obj_path)){
        fs::create_directory(obj_path);
    }
    
    struct settings settings;
    if(read_settings(settings, settings_map) == false){
        return false;
    }
    
    std::vector<string> base_cmd;
    base_cmd.emplace_back(settings.compiler);

    base_cmd.push_back("-MMD");
    base_cmd.push_back("-MP");
    base_cmd.insert(base_cmd.end(), settings.compile_flags.begin(), settings.compile_flags.end());
    //compileflags

    std::vector<fs::path> includes{ get_include_dirs(cwd) };
    for(const auto& dir: includes){
        base_cmd.push_back("-I" + dir.string());
    }

    bool needs_link = false;
    std::vector<fs::path> obj_files;

    std::vector<fs::path> src_files {get_files_in_dir(cwd, settings.src_extensions)};

    for(const auto& src : src_files){
        fs::path relative = fs::relative(src, cwd / src_dir);
        fs::path dep_file{obj_path / relative.replace_extension(".d")};
        fs::path obj_file{obj_path / relative.replace_extension(".o")};
        
        obj_files.push_back(obj_file);

        if(needs_recompilation(obj_file, dep_file)){
            if(compile_file(base_cmd, src, obj_file) == false){
                return false;
            }
            needs_link = true;
        }
        
    }

    if(needs_link){
        link_executable(obj_files, settings);
    }

    return true;
}

bool read_settings(struct settings& settings, const std::unordered_map<string, string>& settings_map){
    
    auto elem{ settings_map.find(setting_compiler)};
    if(elem == settings_map.end()){
        //Change to print if --verbose
        std::cerr << "\"" << setting_compiler << "\" option missing in settings file";
        return false;
    }
    settings.compiler = elem->second;
    
    elem = settings_map.find(setting_comp_flags);
    if(elem == settings_map.end()){
        //Change to print if --verbose
        std::cerr << "\"" << setting_comp_flags << "\" option missing in settings file";
        return false;
    }
    settings.compile_flags = string_to_vector(elem->second);

    elem = settings_map.find(setting_src_extension);
    if(elem == settings_map.end()){
        //Change to print if --verbose
        std::cerr << "\"" << setting_src_extension << "\" option missing in settings file";
        return false;
    }
    settings.src_extensions = string_to_vector(elem->second);

    return true;
}

std::vector<fs::path> read_dependencies(const fs::path& dep_file)
{
    std::ifstream file(dep_file);

    if (!file)
        return {};

    std::string content{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };

    std::replace(content.begin(), content.end(), '\\', ' ');

    auto colon = content.find(':');

    if (colon == std::string::npos)
        return {};

    std::istringstream stream{content.substr(colon + 1)};

    std::vector<std::filesystem::path> deps;
    std::string dep;

    while (stream >> dep){
        if(dep.empty() || dep.back() == ':')
            continue;

        deps.emplace_back(dep);
    }

    return deps;
}

std::vector<fs::path> get_include_dirs(const fs::path& cwd){

    std::vector<fs::path> dirs;

    fs::path include{ cwd / include_dir };
    if(!fs::is_directory(include)){
        return dirs;
    }

    dirs.push_back(include);

    for(const auto& entry : fs::recursive_directory_iterator(include)){
        if(entry.is_directory()){
            dirs.push_back(entry);
        }
    }

    return dirs;
}

bool compile_file(const std::vector<string>& base_cmd, const fs::path& src, const fs::path& obj){
    std::vector<string> cmd {base_cmd};

    cmd.push_back("-c");
    cmd.push_back(src.string());
    cmd.push_back("-o");
    cmd.push_back(obj.string());
    
    std::cout << "Compiling file: " << src.filename() << "\n";
    execute_and_wait(cmd);

    return true;
}

bool needs_recompilation(const fs::path& obj_file, const fs::path& dep_file){
    if(!fs::exists(obj_file)){
        return true;
    }

    if(!fs::exists(dep_file)){
        return true;
    }

    auto obj_time = fs::last_write_time(obj_file);

    for(const auto& dep : read_dependencies(dep_file)){
        if(!fs::exists(dep)){
            return true;
        }

        if(fs::last_write_time(dep) > obj_time){
            return true;
        }
    }

    return false;
}

void link_executable(const std::vector<fs::path>& obj_files, const struct settings& settings){
    std::vector<string> cmd;

    cmd.emplace_back(settings.compiler);
    
    cmd.insert(cmd.end(), settings.compile_flags.begin(), settings.compile_flags.end());

    for(const auto& obj : obj_files){
        cmd.push_back(obj.string());
    }
    cmd.push_back("-o");
    cmd.push_back("build/app_name");

    std::cout << "Linking executable\n";

    execute_and_wait(cmd);
}

