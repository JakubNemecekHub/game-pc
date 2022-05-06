#pragma once

#include <stdio.h>  // FILE
#include <string>

class LogManager
{
private:
    static LogManager* singleton_;
    LogManager() {};
    FILE* file_log;
    FILE* file_error;
public:
    static LogManager* GetInstance();
    ~LogManager() {};
    void startUp();
    void shutDown();
    std::string get_time_stamp();
    // Variadic template function must be in the header file,
    // otherwise we get linker error. Why?
    template<typename... Messages>
    void log_message(Messages... messages)
    {
        std::cout << get_time_stamp() << " | ";
        ((std::cout << std::forward<Messages>(messages)), ...);
        std::cout << std::endl;
    }
    template<typename... Messages>
    void log_error(Messages&&... messages)
    {
        std::cerr << get_time_stamp() << " | ";
        ((std::cerr << std::forward<Messages>(messages)), ...);
        std::cerr << std::endl;
    }
};