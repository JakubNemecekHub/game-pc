#pragma once

#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "Sprite.hpp"
#include "GameObject.hpp"
#include "Item.hpp"
#include "Door.hpp"
#include "HotSpot.hpp"
#include "Ambient.hpp"
#include "../math/Polygon.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/ItemManager.hpp"
#include "../managers/AssetManager.hpp"


class RoomAnimations
{
private:

    std::vector<Ambient> animations_;

public:

    RoomAnimations() {};
    ~RoomAnimations() {};

    void load(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt);

    Ambient* get_animation(int x, int y);

};


class Room
{
private:

    // Fields

    Sprite*        sprite_;        // Background texture.
    Polygon        walk_area_;     // A polygon representing the walk area.
    RoomAnimations animations_;    // Manages ambient animations.
    SDL_Surface*   click_map_;     // A bitmap of hot-spots.

    /*
        Raw pointers to Items( Store and managed by ItemManager) are in separate map.
        Other GameObjects are in a map of maps of specific types of GameObjects. Should be:
        - hot_spots
        - doors
        - other -> just in case
    */
    std::unordered_map<std::string, Item*> items_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<GameObject>>> objects_;

    // Debug

    bool visible_click_map_  { false };
    bool visible_walk_area_  { false };
    bool visible_item_debug_      { false };
    bool visible_hot_spot_debug_      { false };

    // Methods

    auto relative_coordinates(int x, int y);
    Uint32 get_mapped_object_id_(int x, int y);

public:

    Room() {};
    Room(YAML::Node data, ItemManager* items, AssetManager* assets);
    ~Room();

    // Methods

           void update(RenderManager* renderer, int dt);
           bool walkable(int x, int y);
    GameObject* get_object(int x, int y);
           void remove_item(std::string id);

    // DEBUG

    inline void toggle_click_map()      { visible_click_map_ = !visible_click_map_; }
    inline void toggle_walk_area()      { visible_walk_area_ = !visible_walk_area_; }
    inline void toggle_item_debug()     { visible_item_debug_ = !visible_item_debug_; visible_hot_spot_debug_ = false; }
    inline void toggle_hot_spot_debug() { visible_hot_spot_debug_ = !visible_hot_spot_debug_; visible_item_debug_ = false; }

    GameObject* get_item(int x, int y);
    GameObject* get_hot_spot(int x, int y);

};