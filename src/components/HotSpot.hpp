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

    void accept_click(Gameplay::GameplayState* handler, SDL_Event event) override;
    void accept_over(Gameplay::GameplayState* handler, SDL_Event event) override;
    void accept_drag(Gameplay::GameplayState* handler, SDL_Event event) override;

};