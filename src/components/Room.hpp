#pragma once

#include <unordered_map>
#include <memory>   // unique_ptr
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

// #include "Ambient.hpp"
#include "Animation.hpp"
#include "Item.hpp"
#include "Door.hpp"
#include "HotSpot.hpp"
#include "../math_objects/PolygonObject.hpp"
#include "../math/Polygon.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/ItemManager.hpp"
#include "../managers/TextureManager.hpp"


class RoomAnimations
{
private:

    std::vector<Animation*> animations_;

public:

    RoomAnimations() {};
    ~RoomAnimations() {};

    void load(YAML::Node data, TextureManager* textures);
    void update(RenderManager* renderer, int dt);

};


/*
    Room has its own background texture. All ambient animations are handled
    by RoomManager.
    Room also has a click_map, which is a bitmap (.bmp) loaded into surface.
    Different active spots are represented by different colors on the click_map.

    TO DO: Should prepare a list af available colours. Or handle them "dynamically"?
    mouse click -> get color from click_map -> find color in some unordered_map
    or something like that
*/
class Room
{
private:

    // Fields

    Texture*                                    texture_;                    // Background texture
    Polygon                                     walk_area_;                  // A polygon representing the walk area
    RoomAnimations                              animations_;                 // Manages ambient animations
    SDL_Surface*                                click_map_;                  // A bitmap of hot-spots.

    std::unordered_map<Uint32, HotSpot>         hot_spots_;
    std::unordered_map<Uint32, Door>            doors_;
    std::unordered_map<std::string, Item*>      items_;                     // After clicking it is necessary to check all items, so vector would be better
                                                                            // but vector seems to bring some problem with copy constructor.

    // Debug

    std::vector<PolygonObject*>                 screen_items_;               // A visual representation of items_'s click map
    bool                                        visible_click_map_ {false};
    Texture*                                    click_map_texture_;          // A visual representation of click_map_
    bool                                        visible_walk_area_ {false};
    std::unique_ptr<PolygonObject>              screen_walk_area_;           // A visual representation of walk_area_

    // Methods

    void get_room_coordinates_(int x, int y, int* world_x, int* world_y);
    Uint32 get_mapped_object_(int x, int y);

    // Debug Methods

    // void create_click_map_texture();
    void create_screen_walk_area_();

public:

    Room() {};
    Room(YAML::Node data, RenderManager* renderer, ItemManager* items, TextureManager* textures);
    ~Room();

    // Methods

    // void load(YAML::Node data);
    void update(RenderManager* renderer, int dt);
    bool walkable(int x, int y);
    HotSpot* get_hot_spot(int x, int y);
    Door* get_door(int x, int y);
    Item* get_item(int x, int y);
    void remove_item(std::string id);

    // Debug

    void toggle_click_map();
    void toggle_walk_area();

};