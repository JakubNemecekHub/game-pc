#pragma once

#include <stdio.h>  // FILE

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
};