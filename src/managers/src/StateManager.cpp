#include "../StateManager.hpp"

#include "../logic/State.hpp"


StateManager::StateManager()
    : state_{nullptr}, next_state_{nullptr} {}


void StateManager::start(State* state, Managers* managers)
{
    state->enter(managers);
    state_ = state;
}


void StateManager::next(State* state)
{
    // TO DO: Handle States priority?
    next_state_ = state;
}


void StateManager::change(Managers* managers)
{
    if ( !next_state_ ) return;
    state_->exit();
    next_state_->enter(managers);
    state_ = next_state_;
    next_state_ = nullptr;
}


State* StateManager::state()
{
    return state_;
}