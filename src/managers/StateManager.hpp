#pragma once

#include <string>

#include "../logic/State.hpp"
class Managers;


// TO DO: hardcode default state into cpp as lua script string
class StateManager
{
private:
    State state_;
    std::string next_state_;
public:
    StateManager();
    void start(Managers* managers, std::string state);  
    void next(std::string state);
    void change();
    inline State* state() { return &state_; }
};