#ifndef _BASE_MODULE_H_
#define _BASE_MODULE_H_

#include <unordered_map>
#include <filesystem>

#include "types.hpp"

class BaseModule{
    public:
        virtual bool read_settings(const std::unordered_map<string, string>& settings_map) = 0;
        virtual bool create_project_structure() = 0;
        virtual bool compile() = 0;
        virtual bool link() = 0;

        //virtual int execute(); to execute the linked program when arguments are properly implemented

        explicit BaseModule(const std::filesystem::path& _cwd): cwd{_cwd} {}

    protected:
        std::filesystem::path cwd;
};


#endif