#include "c_cpp_module.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

const string src_dir{"src"};
const string obj_dir{"obj"};
const string include_dir{"include"};

std::vector<fs::path> read_dependencies(const fs::path& dep_file);
std::vector<fs::path> get_include_dirs(const fs::path& cwd);

void compile_file(const string& base_cmd, const fs::path& src, const fs::path& obj);
bool needs_recompilation(const fs::path& obj_file, const fs::path& dep_file);

void compile(const std::vector<std::filesystem::path>& files, const std::filesystem::path& cwd, const std::vector<string>& compile_flags){

    fs::path obj_path { cwd / obj_dir };

    if(!fs::exists(obj_path) || !fs::is_directory(obj_path)){
        fs::create_directory(obj_path);
    }


    std::vector<fs::path> includes{ get_include_dirs(cwd) };


    string base_cmd { "g++ -MMD -MP" };
    for(const auto& flag: compile_flags){
        base_cmd += " " + flag;
    }
    for(const auto& dir: includes){
        base_cmd += " -I" + dir.string();
    }

    bool needs_link = false;
    std::vector<fs::path> obj_files;

    for(const auto& file : files){
        fs::path relative = fs::relative(file, cwd / src_dir);
        fs::path dep_file{obj_path / relative.replace_extension(".d")};
        fs::path obj_file{obj_path / relative.replace_extension(".o")};
        
        obj_files.push_back(obj_file);

        if(needs_recompilation(obj_file, dep_file)){
            compile_file(base_cmd, file, obj_file);
            needs_link = true;
        }
    }

    if(needs_link){
        string cmd{"g++"};

        for(const auto& obj : obj_files){
            cmd += " " + obj.string();
        }

        cmd += " -o build/app_name";

        std::system(cmd.c_str());
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