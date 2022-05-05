#include "../LogManager.hpp"

#include <iostream> // cout, cerr
#include <cstdio>   // freopen, fclose


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
    std::cout << "Starting Log Manager." << std::endl;
    file_log = std::freopen("log.txt", "w", stdout);
    file_error = std::freopen("error.txt", "w", stderr);
}


void LogManager::shutDown()
{
    std::cout << "Shuting down Log Manager." << std::endl;
    std::fclose(file_log);
    std::fclose(file_error);
}