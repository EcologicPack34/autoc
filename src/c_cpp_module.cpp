#include "c_cpp_module.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "file_utils.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

const string src_dir{"src"};
const string obj_dir{"obj"};
const string include_dir{"include"};

/*-----DEFINITIONS------*/

std::vector<fs::path> read_dependencies(const fs::path& dep_file);
std::vector<fs::path> get_include_dirs(const fs::path& cwd);

void compile_file(const string& base_cmd, const fs::path& src, const fs::path& obj);
bool needs_recompilation(const fs::path& obj_file, const fs::path& dep_file);

void link_executable(const std::vector<fs::path>& obj_files, const string& compiler, const string& compile_flags);

/*-----------IMPLEMENTATION------------*/

void compile(const std::filesystem::path& cwd, const std::unordered_map<string, string>& settings){

    fs::path obj_path { cwd / obj_dir };
    if(!fs::exists(obj_path) || !fs::is_directory(obj_path)){
        fs::create_directory(obj_path);
    }
    

    string compiler{unorderedmap_get_or_default<string,string>(settings, "compiler", "g++")};
    string base_comp_cmd {compiler};
    base_comp_cmd += " -MMD -MP ";
    
    string compile_flags{unorderedmap_get_or_default<string, string>(settings, "compile_flags", "")};
    base_comp_cmd += compile_flags;

    std::vector<fs::path> includes{ get_include_dirs(cwd) };
    for(const auto& dir: includes){
        base_comp_cmd += " -I" + dir.string();
    }

    bool needs_link = false;
    std::vector<fs::path> obj_files;

    std::vector<fs::path> src_files {get_files_in_dir(cwd, {".cpp" , ".c"})};

    for(const auto& src : src_files){
        fs::path relative = fs::relative(src, cwd / src_dir);
        fs::path dep_file{obj_path / relative.replace_extension(".d")};
        fs::path obj_file{obj_path / relative.replace_extension(".o")};
        
        obj_files.push_back(obj_file);

        if(needs_recompilation(obj_file, dep_file)){
            compile_file(base_comp_cmd, src, obj_file);
            needs_link = true;
        }
    }

    if(needs_link){
        link_executable(obj_files, compiler, compile_flags);
    }

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

void compile_file(const string& base_cmd, const fs::path& src, const fs::path& obj){
    string cmd {base_cmd};

    cmd += " -c " + src.string();
    cmd += " -o " + obj.string();

    std::cout << cmd << "\n";

    std::system(cmd.c_str());
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

void link_executable(const std::vector<fs::path>& obj_files, const string& compiler, const string& compile_flags){
    string cmd{compiler};

    cmd += " " + compile_flags;

    for(const auto& obj : obj_files){
        cmd += " " + obj.string();
    }

    cmd += " -o build/app_name";

    std::system(cmd.c_str());
}

