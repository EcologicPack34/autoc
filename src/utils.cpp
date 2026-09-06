#include "utils.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int execute_and_wait(const std::vector<std::string>& cmd){

    pid_t pid = fork();
    if(pid < 0) return -1;
    else if(pid == 0){
        std::vector<char*> argv;
        //no usar const para bucle por implicit type conversion
        for(auto& token : cmd){
            argv.push_back((char *)token.data());
        }
        argv.push_back(nullptr);

        for(const auto& elem : cmd){
            std::cout << elem << " ";
        }
        std::cout << "\n";

        std::cout.flush();
        execvp(argv.at(0), argv.data());
    }else{
        int status;
        waitpid(pid, &status, 0);
        std::cout << "\n";

        if(WIFEXITED(status)){
            return WEXITSTATUS(status);
        }
    }

    return -1;
}