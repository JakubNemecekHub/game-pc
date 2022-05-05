#include "../LogManager.hpp"

#include <iostream>


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
}


void LogManager::shutDown()
{
    std::cout << "Shuting down Log Manager." << std::endl;
}