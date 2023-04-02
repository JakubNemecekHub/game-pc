#pragma once

#include <iostream>
#include <stdio.h>  // FILE
#include <string>

#include <yaml-cpp/yaml.h>


class LogManager
{
private:

    std::string file_log_name_;
    std::string file_error_name_;
    FILE* file_log_;
    FILE* file_error_;

public:

    LogManager() {};
    LogManager(YAML::Node ini);

    bool startUp();
    bool shutDown();

    std::string get_time_stamp();
    // Variadic template function must be in the header file,
    // otherwise we get linker error. Why?
    template<typename... Messages>
    void log(Messages... messages)
    {
        std::cout << get_time_stamp() << " | ";
        ((std::cout << std::forward<Messages>(messages) << " "), ...);
        std::cout << std::endl;
    }
    template<typename... Messages>
    void error(Messages&&... messages)
    {
        std::cerr << get_time_stamp() << " | ";
        ((std::cerr << std::forward<Messages>(messages) << " "), ...);
        std::cerr << std::endl;
    }
};