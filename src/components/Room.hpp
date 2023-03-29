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
#include "../math/Polygon.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/ItemManager.hpp"
#include "../managers/AssetManager.hpp"


class RoomAnimations
{
private:

    std::vector<Sprite*> animations_;

public:

    RoomAnimations() {};
    ~RoomAnimations() {};

    void load(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt);

};


class Room
{
private:

    // Fields

    Sprite*                                     sprite_;        // Background texture
    Polygon                                     walk_area_;     // A polygon representing the walk area
    RoomAnimations                              animations_;    // Manages ambient animations
    SDL_Surface*                                click_map_;     // A bitmap of hot-spots.

    std::unordered_map<Uint32, HotSpot>         hot_spots_;
    std::unordered_map<Uint32, Door>            doors_;
    std::unordered_map<std::string, Item*>      items_;         // After clicking it is necessary to check all items, so vector would be better
                                                                // but vector seems to bring some problem with copy constructor.

    // Debug

    bool visible_click_map_ { false };
    bool visible_walk_area_ { false };
    bool visible_item_click_map_ { false };
    bool visible_item_vector_ { false };

    // Methods

    void get_room_coordinates_(int x, int y, int* world_x, int* world_y);
    Uint32 get_mapped_object_(int x, int y);

public:

    Room() {};
    Room(YAML::Node data, RenderManager* renderer, ItemManager* items, AssetManager* assets);
    ~Room();

    // Methods

           void update(RenderManager* renderer, int dt);
           bool walkable(int x, int y);
    GameObject* get_object(int x, int y);
           void remove_item(std::string id);

    // Debug

    inline void toggle_click_map()      { visible_click_map_ = !visible_click_map_;           }
    inline void toggle_walk_area()      { visible_walk_area_ = !visible_walk_area_;           }
    inline void toggle_item_click_map() { visible_item_click_map_ = !visible_item_click_map_; }
    inline void toggle_item_vector()    { visible_item_vector_ = !visible_item_vector_; }

};