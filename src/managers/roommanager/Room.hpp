#pragma once

#include <unordered_map>
#include <memory>   // unique_ptr
#include <vector>

#include <SDL2/SDL.h>

#include "Ambient.hpp"
#include "GameObject.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"


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

    std::unique_ptr<Texture> texture;                   // Background texture
    Polygon walk_area;                                  // A polygon representing the walk area
    bool visible_walk_area {false};
    std::unique_ptr<PolygonObject> screen_walk_area;    // a visual representation of walk_area
    SDL_Surface* click_map;                             // A bitmap of hot-spots.
    bool visible_click_map {false};
    std::unique_ptr<Texture> click_map_texture;         // A visual representation of click_map
    Ambient ambient;                                    // Manager of ambient animations.
    std::unordered_map<Uint32, AmbientObject> objects_ambient;
    std::unordered_map<Uint32, Door> objects_door;
    std::unordered_map<Uint32, Item> items;             // After clicking it is necessary to check all items, so vector would be better
                                                        // but vector seems to bring some problem with copy constructor.
    std::vector<PolygonObject*> screen_items;            // A visual representation of item's click map

    // Methods

    void get_room_coordinates(int x, int y, int* world_x, int* world_y);

    // Debug Methods

    void create_click_map_texture();
    void create_screen_walk_area();

public:

    Room() {};  // Empty Constructor, use load
    ~Room();    // Destructor

    // Methods

    void load(json room_meta);
    bool point_in_polygon(int x, int y);
    void update(int dt);
    Uint32 get_mapped_object(int x, int y);
    AmbientObject* get_ambient_object(Uint32 id);
    Door* get_door(Uint32 id);
    Item* item_get(int x, int y);

    // Methods to test various things

    void toggle_click_map();
    void toggle_walk_area();
};