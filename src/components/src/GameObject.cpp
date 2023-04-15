#include "../GameObject.hpp"

#include <iostream>


GameObject::GameObject(std::string id, bool state)
    : id_{id}, state_{state} {}


std::string GameObject::id()
{
    std::cout << "Hold on.\n";
    return id_;
}
bool GameObject::state() { return state_; }
void GameObject::state(bool new_state) { state_ = new_state; }


// DEBUG

void GameObject::show_attributes() { debug_ = true; }