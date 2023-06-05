#pragma once

#include <iostream>
#include <string>
#include <chrono>

class MicroTimer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point;
public:
    MicroTimer()
    {
        start_time_point = std::chrono::high_resolution_clock::now();
    }
    ~MicroTimer()
    {
        auto end_time_point = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_point).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

        auto duration = end - start;
        std::cout << duration << "\n"; 
    }
};