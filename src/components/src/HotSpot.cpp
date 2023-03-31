#include "../HotSpot.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"


// HotSpot::HotSpot() {}


HotSpot::HotSpot(YAML::Node data)
{
    state_ = data["state"].as<bool>();
    observations_ = data["observations"].as<std::vector<std::string>>();
    use_observations_ = data["use_observations"].as<std::vector<std::string>>();
}


std::string HotSpot::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string HotSpot::use_observation()
{
    return use_observations_[rand() % use_observations_.size()];
}

void HotSpot::accept(Gameplay::GameplayState* handler, Mouse::click mouse) { handler->visit(this, mouse); }