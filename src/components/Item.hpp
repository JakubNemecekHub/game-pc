#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"
#include "../managers/AssetManager.hpp"
#include "Serial.hpp"
#include "Sprite.hpp"
#include "../math/Polygon.hpp"


class Item : public GameObject, public Serial
{
private:

    bool                        lock_;
    std::vector<std::string>    observations_;
    std::string                 pick_observation_;
    Polygon                     click_area_;
    Sprite*                     sprite_;
    
public:

    Item(YAML::Node data, AssetManager* assets);

    void update(RenderManager* renderer, int dt) override;

    bool        clicked(float x, float y) override;
    std::string observation();
    std::string pick_observation();
    bool        lock();
    void        lock(bool new_lock);
    Sprite*     sprite();
    Polygon*    click_area();

    // Position

    void x(float x);
    void y(float y);
    void move (float dx, float dy);

    // Game logic

    void accept_click(State* handler, int x, int y, bool r) override;
    void accept_over(State* handler, int x, int y) override;
    void accept_drag(State* handler, SDL_Event& event) override;

    // Serialization

    void write(SerializationManager* io) override;
    
};