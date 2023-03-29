#pragma once

#include <string>
#include <vector>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "../managers/AssetManager.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "../math/Polygon.hpp"


class Item : public GameObject
{
private:
    std::string                 id_;
    bool                        state_;
    bool                        lock_;
    std::vector<std::string>    observations_;
    std::string                 pick_observation_;
    Polygon                     click_area_;
    Sprite*                     sprite_;
public:

    Item(YAML::Node data, AssetManager* assets);

    void update(RenderManager* renderer, int dt);

    bool        clicked(int x, int y);
    std::string observation();
    std::string pick_observation();
    std::string id();
    bool        state();
    void        state(bool new_state);
    bool        lock();
    void        lock(bool new_lock);
    Sprite*     sprite();
    Polygon*    click_area();

    void accept(Game* handler, Mouse::click mouse) override;
    
};