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
public:
    bool visible_click_map {false};
    bool visible_walk_area {false};
    std::unique_ptr<Texture> texture;
    Polygon walk_area;
    std::unique_ptr<PolygonObject> screen_walk_area;
    SDL_Surface* click_map;                             // A bitmap of hot-spots.
    std::unique_ptr<Texture> click_map_texture;
    std::unordered_map<Uint32, std::string> doors;      // A map of doors and their destinations.
    std::unordered_map<Uint32, std::string> actions;    // A map of hot-spots indexes and their actions.
    Ambient ambient;                                    // Manager of ambient animations. Is it hecessary to have separate class for this?
    // Constructor - move implementation to .cpp
    Room() {};
    void load(json room_meta);
    // Destructor
    ~Room();

    // Methods

    void handleEvents(SDL_Event event);
    void update(int dt);
    Uint32 get_mapped_object(int x, int y);

    // Methods to test various thigs

    void create_click_map_texture();
    void toggle_click_map();
    // void render_click_map();
    // void hide_click_map();
    void create_screen_walk_area();
    void toggle_walk_area();
};