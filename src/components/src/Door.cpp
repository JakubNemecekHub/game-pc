#include "../Door.hpp"

#include <stdlib.h> // rand


// Door::Door() {}


Door::Door(YAML::Node data)
{
    state_ = data["state"].as<bool>();
    locked_ = data["locked"].as<bool>();
    target_ = data["target"].as<std::string>();
    observations_ = data["observations"].as<std::vector<std::string>>();
    locked_observations_ = data["locked_observations"].as<std::vector<std::string>>();
    key_id_ = data["key_id"].as<std::string>();
}


std::string Door::target() { return target_; }
bool Door::locked() { return locked_; }
void Door::unlock() { locked_ = false; }


std::string Door::get_observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Door::get_locked_observation()
{
    return locked_observations_[rand() % locked_observations_.size()];
}


bool Door::state() { return state_; }
void Door::state(bool new_state) { state_ = new_state; }


std::string Door::key_id() { return key_id_; }

void Door::accept(Visitor* visitor) { visitor->visit_door(this); }