#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"
#include "Serial.hpp"

class State;
class AssetManager;
class RenderManager;
class SerializationManager;


class Item : public GameObject, public Serial
{
private:

    bool                        used_;
    std::vector<std::string>    observations_;
    std::string                 pick_observation_;

public:

    Item(YAML::Node data, AssetManager* assets);

    void update(RenderManager* renderer, int dt) override;

    std::string observation();
    std::string pick_observation();
    bool        is_used();
    void        set_use(bool flag);

    // Game logic

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;
    void accept_drag(State* handler, Mouse::Status mouse) override;

    // Serialization

    void write(SerializationManager* io) override;
    
};