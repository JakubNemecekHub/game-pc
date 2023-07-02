#pragma once

#include <string>

#include "../logic/State.hpp"
class Managers;
class WindowManager;


// TO DO 3: hardcode default state into cpp as lua script string
class StateManager
{
private:

    WindowManager* window_;

    State state_;
    std::string next_state_;

public:

    StateManager();
    void start(Managers* managers, std::string state);  
    void next(std::string state);
    void change();
    inline State* state() { return &state_; }

};