#include "c_cpp_module.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "file_utils.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

static const string setting_compiler{"compiler"};
static const string setting_comp_flags{"compile_flags"};
static const string setting_src_extension{"src_extension"};

static const string setting_src_dir{"src_directory"};
static const string setting_obj_dir{"obj_directory"};
static const string setting_include_dir{"include_directory"};
static const string setting_build_dir{"build_directory"};

static const string setting_exec_name{"exec_name"};

/*-----DEFINITIONS------*/

struct settings{
    std::string_view compiler;
    std::vector<string> compile_flags;
    std::vector<string> src_extensions;

    std::string_view src_dir;
    std::string_view obj_dir;
    std::string_view include_dir;
    std::string_view build_dir;

    std::string_view exec_name;
};

std::vector<fs::path> read_dependencies(const fs::path& dep_file);
std::vector<fs::path> get_include_dirs(const fs::path& cwd);

bool compile_file(const std::vector<string>& base_cmd, const fs::path& src, const fs::path& obj);
bool needs_recompilation(const fs::path& obj_file, const fs::path& dep_file);

/*-----------IMPLEMENTATION------------*/

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

std::vector<fs::path> get_include_dirs(const fs::path& include_path){

    std::vector<fs::path> dirs;
    dirs.push_back(include_path);

    for(const auto& entry : fs::recursive_directory_iterator(include_path)){
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

bool C_CPP_Module::read_settings(const std::unordered_map<string, string>& settings_map){
    auto get_setting = [&](const string& key) -> const string* {
        auto it = settings_map.find(key);

        if (it == settings_map.end()) {
            std::cerr << '"' << key << "\" option missing in settings file\n";
            return nullptr;
        }

        return &it->second;
    };

    auto compiler = get_setting(setting_compiler);
    if (!compiler) return false;
    this->compiler = *compiler;

    auto compile_flags = get_setting(setting_comp_flags);
    if (!compile_flags) return false;
    this->compile_flags = string_to_vector(*compile_flags);

    auto src_extension = get_setting(setting_src_extension);
    if (!src_extension) return false;
    this->src_extensions = string_to_vector(*src_extension);

    auto src_dir = get_setting(setting_src_dir);
    if (!src_dir) return false;
    this->src_dir = *src_dir;

    auto obj_dir = get_setting(setting_obj_dir);
    if (!obj_dir) return false;
    this->obj_dir = *obj_dir;

    auto include_dir = get_setting(setting_include_dir);
    if (!include_dir) return false;
    this->include_dir = *include_dir;

    auto build_dir = get_setting(setting_build_dir);
    if (!build_dir) return false;
    this->build_dir = *build_dir;

    auto exec_name = get_setting(setting_exec_name);
    if(!exec_name) return false;
    this->exec_name = *exec_name;

    return true;
}

bool C_CPP_Module::create_project_structure(){
    fs::path obj_path { cwd / this->obj_dir };
    if(!fs::exists(obj_path) || !fs::is_directory(obj_path)){
        fs::create_directory(obj_path);
    }

    fs::path src_path { cwd / this->src_dir };
    if(!fs::exists(src_path) || !fs::is_directory(src_path)){
        fs::create_directory(src_path);
    }

    fs::path include_path { cwd / this->include_dir };
    if(!fs::exists(include_path) || !fs::is_directory(include_path)){
        fs::create_directory(include_path);
    }

    fs::path build_path { cwd / this->build_dir };
    if(!fs::exists(build_path) || !fs::is_directory(build_path)){
        fs::create_directory(build_path);
    }

    return true;
}

bool C_CPP_Module::compile(){
    fs::path obj_path{ cwd / this->obj_dir};
    fs::path src_path{ cwd / this->src_dir};
    fs::path include_path{ cwd / this->include_dir};

    std::vector<string> base_cmd;
    base_cmd.emplace_back(this->compiler);

    base_cmd.push_back("-MMD");
    base_cmd.push_back("-MP");
    //Insert compile flags into command
    base_cmd.insert(base_cmd.end(), this->compile_flags.begin(), this->compile_flags.end());

    std::vector<fs::path> includes{ get_include_dirs(include_path) };
    for(const auto& dir: includes){
        base_cmd.push_back("-I" + dir.string());
    }

    std::vector<fs::path> obj_files;
    std::vector<fs::path> src_files {get_files_in_dir(cwd, this->src_extensions)};
    for(const auto& src : src_files){
        fs::path relative = fs::relative(src, src_path);
        fs::path dep_file{obj_path / relative.replace_extension(".d")};
        fs::path obj_file{obj_path / relative.replace_extension(".o")};
        
        obj_files.push_back(obj_file);

        //Add better error checking
        if(needs_recompilation(obj_file, dep_file)){
            if(compile_file(base_cmd, src, obj_file) == false){
                return false;
            }
        }
    }

    return true;
}

bool C_CPP_Module::link(){
    std::vector<string> cmd;

    cmd.emplace_back(this->compiler);
    //Insert compile flags into command
    cmd.insert(cmd.end(), this->compile_flags.begin(), this->compile_flags.end());

    fs::path obj_path{ cwd / this->obj_dir};
    fs::path src_path{ cwd / this->src_dir};
    fs::path exec_path{ cwd / this->build_dir / this->exec_name};

    bool needs_link = fs::exists(exec_path) ? false : true;

    std::vector<fs::path> src_files {get_files_in_dir(cwd, this->src_extensions)};
    for(const auto& src : src_files){
        fs::path relative = fs::relative(src, src_path);
        fs::path obj_file{obj_path / relative.replace_extension(".o")};

        cmd.push_back(obj_file);

        auto compare = compare_file_mod_dates(exec_path, obj_file);
        if(!compare || *compare < 0){
            needs_link = true;
        }
    }

    if(!needs_link){
        std::cout << "Executable is up to date\n";
        return false;
    }

    cmd.push_back("-o");
    cmd.push_back(this->build_dir + "/" + this->exec_name);

    std::cout << "Linking executable " << this->exec_name << "\n";

    //add error checking
    execute_and_wait(cmd);

    return true;
}

