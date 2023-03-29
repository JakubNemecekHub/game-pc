#include "../HotSpot.hpp"

#include <stdlib.h> // rand


// HotSpot::HotSpot() {}


HotSpot::HotSpot(YAML::Node data)
{
    state_ = data["state"].as<bool>();
    observations_ = data["observations"].as<std::vector<std::string>>();
    use_observations_ = data["use_observations"].as<std::vector<std::string>>();
}


std::string HotSpot::get_observation()
{
    return observations_[rand() % observations_.size()];
}


std::string HotSpot::get_use_observation()
{
    return use_observations_[rand() % use_observations_.size()];
}

void HotSpot::accept(Visitor* visitor, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse)  { visitor->visit(this, text_, player_, room_, mouse); }