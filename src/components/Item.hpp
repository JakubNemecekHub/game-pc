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


class Item : public GameObject, public Serial, public std::enable_shared_from_this<Item>
{
private:

    bool                        lock_;
    std::vector<std::string>    observations_;
    std::string                 pick_observation_;
    Polygon                     click_area_;
    Sprite*                     sprite_;
    
public:

    Item(YAML::Node data, AssetManager* assets);
    std::shared_ptr<Item> Get() { return shared_from_this(); }

    void update(RenderManager* renderer, int dt);

    bool        clicked(int x, int y);
    std::string observation();
    std::string pick_observation();
    bool        lock();
    void        lock(bool new_lock);
    Sprite*     sprite();
    Polygon*    click_area();

    // Position

    void x(int x);
    void y(int y);
    void move (int dx, int dy);

    // Game logic

    void accept_click(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_over(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) override;

    // Serialization

    void write(SerializationManager* io) override;
    
};