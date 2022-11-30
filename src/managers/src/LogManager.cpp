#include "../LogManager.hpp"

#include <iostream> // cout, cerr
#include <cstdio>   // freopen, fclose
#include <chrono>   // system_clock::now
#include <ctime>    // time_t, ctime
#include <stdio.h>
#include <stdarg.h> // for function with variable number of arguments


LogManager::LogManager(YAML::Node ini)
{
    file_log_name_ = ini["file_log"].as<std::string>();
    file_error_name_ = ini["file_error"].as<std::string>();
}


bool LogManager::startUp()
{
    file_log_ = std::freopen(file_log_name_.c_str(), "w", stdout);
    file_error_ = std::freopen(file_error_name_.c_str(), "w", stderr);
    log("Started Log Manager.");
    return true;
}


bool LogManager::shutDown()
{
    log("Shutting down Log Manager.");
    std::fclose(file_log_);
    std::fclose(file_error_);
    return true;
}


std::string LogManager::get_time_stamp()
{
    auto time_now = std::chrono::system_clock::now();
    std::time_t time_stamp = std::chrono::system_clock::to_time_t(time_now);
    return std::strtok(std::ctime(&time_stamp), "\n");
}
