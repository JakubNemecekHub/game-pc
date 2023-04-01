#include "../HotSpot.hpp"

#include <stdlib.h> // rand

#include "../logic/States.hpp"


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

void HotSpot::accept_click(Gameplay::GameplayState* handler, Mouse::Transform mouse_transform) { handler->visit_click(this, mouse_transform); }
void HotSpot::accept_over(Gameplay::GameplayState* handler, Mouse::Transform mouse_transform) { handler->visit_over(this, mouse_transform); }