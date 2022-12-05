#pragma once

#include <string>
#include <vector>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "../managers/AssetManager.hpp"
#include "Texture.hpp"
#include "../math/Polygon.hpp"


class Item {
private:
    std::string                 id_;
    bool                        state_;
    bool                        lock_;
    std::vector<std::string>    observations_;
    std::string                 pick_observation_;
    Polygon                     click_area_;
    Texture*                    texture_;
public:

    Item(YAML::Node data, AssetManager* assets);

    // void        load(YAML::Node data);
    bool        clicked(int x, int y);
    std::string get_observation();
    std::string get_pick_observation();
    std::string id();
    bool        state();
    void        state(bool new_state);
    bool        lock();
    void        lock(bool new_lock);
    Texture*    texture();
    Polygon&    click_area();
};