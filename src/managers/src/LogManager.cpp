#include "../LogManager.hpp"

#include <iostream> // cout, cerr
#include <cstdio>   // freopen, fclose
#include <chrono>   // system_clock::now
#include <ctime>    // time_t, ctime
#include <stdio.h>
#include <stdarg.h> // for function with variable number of arguments


LogManager* LogManager::singleton_ = nullptr;


LogManager* LogManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new LogManager();
    }
    return singleton_;
}


void LogManager::startUp()
{
    file_log = std::freopen("log.txt", "w", stdout);
    file_error = std::freopen("error.txt", "w", stderr);
    log_message("Started Log Manager.");
}


void LogManager::shutDown()
{
    log_message("Shutting down Log Manager.");
    std::fclose(file_log);
    std::fclose(file_error);
}


std::string LogManager::get_time_stamp()
{
    auto time_now = std::chrono::system_clock::now();
    std::time_t time_stamp = std::chrono::system_clock::to_time_t(time_now);
    return std::strtok(std::ctime(&time_stamp), "\n");
}
