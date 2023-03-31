#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>   // Uint32

#include "GameObject.hpp"


class HotSpot : public GameObject
{
private:
    // Uint32                      id_;
    bool                        state_;
    std::vector<std::string>    observations_;
    std::vector<std::string>    use_observations_;
public:

    HotSpot(YAML::Node data);
    ~HotSpot() {};

    std::string observation();
    std::string use_observation();

    void accept(State* handler, Mouse::click mouse) override;

};