#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "Ambient.hpp"
#include "WalkArea.hpp"
#include "../math/Polygon.hpp"
#include "../math/Vector2D.hpp"

class Sprite;
class GameObject;
class Item;
class RenderManager;
class ItemManager;
class AssetManager;
class SerializationManager;


class RoomAnimations
{
private:

    std::vector<Ambient> animations_;

public:

    RoomAnimations() {};
    ~RoomAnimations() {};

    void load(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt);

    inline void reserve(size_t count) { animations_.reserve(count); }
    Ambient* get_animation(Vector2D position);

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
    WalkArea walk_area_;

    // Camera
    float camera_zoom_;
    Vector2D camera_position_;

    // DEBUG

    DEBUG_STATE debug_ { DEBUG_STATE::NORMAL };

public:

    // Room() {};
    Room(YAML::Node data, ItemManager* items, AssetManager* assets);
    ~Room() {};

    // Methods

           void update(RenderManager* renderer, int dt);
           bool walkable(Vector2D position);
    GameObject* get_object(Vector2D position);
           void remove_item(std::string id);

    std::tuple<float, Vector2D> door_camera(std::string door_id);

    void save(SerializationManager* io);

    // DEBUG

    inline void toggle_walk_area()      { debug_ = (debug_ == DEBUG_STATE::WALK_AREA) ? DEBUG_STATE::NORMAL : DEBUG_STATE::WALK_AREA; }
    inline void toggle_item_debug()     { debug_ = (debug_ == DEBUG_STATE::ITEM) ? DEBUG_STATE::NORMAL : DEBUG_STATE::ITEM;           }
    inline void toggle_hot_spot_debug() { debug_ = (debug_ == DEBUG_STATE::HOT_SPOT) ? DEBUG_STATE::NORMAL : DEBUG_STATE::HOT_SPOT;   }
    inline void toggle_door_debug()     { debug_ = (debug_ == DEBUG_STATE::DOOR) ? DEBUG_STATE::NORMAL : DEBUG_STATE::DOOR;           }

};
