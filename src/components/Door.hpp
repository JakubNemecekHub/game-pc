#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>   // Uint32

#include "GameObject.hpp"


class Door : public GameObject
{
private:
    // Uint32                      id_;
    bool                        state_;
    bool                        locked_;
    std::string                 target_;
    std::vector<std::string>    observations_;
    std::vector<std::string>    locked_observations_;
    std::string                 key_id_;
public:

    // Door();
    Door(YAML::Node data);
    ~Door() {};

    std::string target();
    bool        locked();
    void        unlock();
    std::string observation();
    std::string locked_observation();
    bool        state();
    void        state(bool _state);
    std::string key_id();

    void accept(Gameplay::GameplayState* handler, Mouse::click mouse) override;

};