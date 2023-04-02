#pragma once

class State;
class Managers;


class StateManager
{
private:
    State* state_;
    State* next_state_;
public:
    StateManager();
    void start(State* state, Managers* managers);
    void next(State* state);
    void change(Managers* managers);
    State* state();
};