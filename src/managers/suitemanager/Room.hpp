#pragma once

#include <unordered_map>
#include <memory>   // unique_ptr

#include <SDL2/SDL.h>

#include "Ambient.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"


/*
    Room has its own background texture. All ambient animations are handled
    by RoomManager.
    Room also has a click_map, which is a bitmap (.bmp) loaded into surface.
    Different active sposts are represented by different colors on the click_map.

    TO DO: Should prepare a list af aviable colours. Or handle them "dynamically"?
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
    std::unordered_map<Uint32, std::string> doors;      // A map of doors and their destinations.
    std::unordered_map<Uint32, std::string> actions;    // A map of hot-spots indexes and their actions.
    Ambient ambient;                                    // Manager of ambient animations.

    // Methods

    void create_click_map_texture();
    void create_screen_walk_area();

public:

    Room() {};  // Empty Constructor, use load
    ~Room();    // Empty

    // Methods

    void load(json room_meta);
    void get_world_coordinates(int x, int y, int* world_x, int* world_y);
    bool point_in_polygon(int x, int y);
    std::string get_action(Uint32 response);
    std::string get_door_target(Uint32 response);
    void update(int dt);
    Uint32 get_mapped_object(int x, int y);

    // Methods to test various thigs

    void toggle_click_map();
    void toggle_walk_area();
};