#include "../StateManager.hpp"

// #include "../logic/States.hpp"


StateManager::StateManager() {}


void StateManager::start(Managers* managers, std::string state)
{
    state_.set(managers);
    state_.enter(state);
}


void StateManager::next(std::string state)
{
    // TO DO: Handle States priority?
    next_state_ = state;
}


void StateManager::change()
{
    if ( next_state_.empty() ) return;
    if ( state_.name() == "exit" && next_state_ == "exit" ) return;
    state_.exit();
    state_.enter(next_state_);
    next_state_.clear();
}