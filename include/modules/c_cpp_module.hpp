#ifndef C_CPP_MODULE_H_
#define C_CPP_MODULE_H_

#include "types.hpp"

#include <vector>
#include <filesystem>
#include <unordered_map>

#include "modules/base_module.hpp"

class C_CPP_Module : public BaseModule{
    public:
        bool read_settings(const std::unordered_map<string, string>& settings_map);
        bool create_project_structure();
        bool compile();
        bool link();

        using BaseModule::BaseModule;
        C_CPP_Module(const C_CPP_Module&) = default;

    private:
        string compiler;
        std::vector<string> compile_flags;
        std::vector<string> src_extensions;

        string src_dir;
        string obj_dir;
        string include_dir;
        string build_dir;

        string exec_name;
};

#endif