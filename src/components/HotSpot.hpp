#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>   // Uint32

#include "Visitor.hpp"
#include "GameObject.hpp"

class Visitor;


class HotSpot : public GameObject
{
private:
    // Uint32                      id_;
    bool                        state_;
    std::vector<std::string>    observations_;
    std::vector<std::string>    use_observations_;
public:

    // HotSpot();
    HotSpot(YAML::Node data);
    ~HotSpot() {};

    std::string get_observation();
    std::string get_use_observation();

    void accept(Visitor* visitor, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse) override;

};