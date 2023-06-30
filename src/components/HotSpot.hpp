#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"

class State;
class AssetManager;
class RenderManager;


class HotSpot : public GameObject
{
protected:

    std::vector<std::string> observations_;
    std::vector<std::string> use_observations_;

public:

    HotSpot(YAML::Node data, AssetManager* assets);
    HotSpot(YAML::Node data, AssetManager* assets, float room_x, float room_y, float room_scale);
    ~HotSpot() {};

    void update(RenderManager* renderer, int dt) override;

    std::string observation();
    std::string use_observation();

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

};
