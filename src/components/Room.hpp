#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "Ambient.hpp"
#include "../math/Polygon.hpp"

class Sprite;
class GameObject;
class Item;
class RenderManager;
class ItemManager;
class AssetManager;


class RoomAnimations
{
private:

    std::vector<Ambient> animations_;

public:

    RoomAnimations() {};
    ~RoomAnimations() {};

    void load(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt);

    inline void reserve(int count) { animations_.reserve(count); }
    Ambient* get_animation(float x, float y);

};


enum DEBUG_STATE
{
    NORMAL,
    WALK_AREA,
    ITEM,
    HOT_SPOT,
    DOOR
};


class Room
{
private:

    // Fields

    Sprite*        sprite_;        // Background texture.
    Polygon        walk_area_;     // A polygon representing the walk area.
    RoomAnimations animations_;    // Manages ambient animations.

    /*
        Raw pointers to Items( Store and managed by ItemManager) are in separate map.
        Other GameObjects are in a map of maps of specific types of GameObjects. Should be:
        - hot_spots
        - doors
        - other -> just in case
    */
    std::unordered_map<std::string, Item*> items_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<GameObject>>> objects_;


    // Methods

    auto relative_coordinates(float x, float y);

    // DEBUG

    DEBUG_STATE debug_ { DEBUG_STATE::NORMAL }; 

public:

    Room() {};
    Room(YAML::Node data, ItemManager* items, AssetManager* assets);
    ~Room() {};

    // Methods

           void update(RenderManager* renderer, int dt);
           bool walkable(float x, float y);
    GameObject* get_object(float x, float y);
           void remove_item(std::string id);

    GameObject* get_item(float x, float y);
    GameObject* get_hot_spot(float x, float y);

    // DEBUG

    inline void toggle_walk_area()      { debug_ = (debug_ == DEBUG_STATE::WALK_AREA) ? DEBUG_STATE::NORMAL : DEBUG_STATE::WALK_AREA;}
    inline void toggle_item_debug()     { debug_ = (debug_ == DEBUG_STATE::ITEM) ? DEBUG_STATE::NORMAL : DEBUG_STATE::ITEM;           }
    inline void toggle_hot_spot_debug() { debug_ = (debug_ == DEBUG_STATE::HOT_SPOT) ? DEBUG_STATE::NORMAL : DEBUG_STATE::HOT_SPOT;   }
    inline void toggle_door_debug()     { debug_ = (debug_ == DEBUG_STATE::DOOR) ? DEBUG_STATE::NORMAL : DEBUG_STATE::DOOR;           }

};