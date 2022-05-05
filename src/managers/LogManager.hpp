#pragma once


class LogManager
{
private:
    static LogManager* singleton_;
    LogManager() {};
public:
    static LogManager* GetInstance();
    ~LogManager() {};
    void startUp();
    void shutDown();
};