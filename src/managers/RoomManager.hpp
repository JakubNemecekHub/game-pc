#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <iostream>
// #include <array>
#include <unordered_map>
#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include "../components/Texture.hpp"
#include "../components/Animation.hpp"
#include "../math_objects/PolygonObject.hpp"
#include "../math/Polygon.hpp"

/*
    OLD !
    When starting up RoomManager, create empty Room (just a field that will hold a pointer to a Room).
    Then implement method LoadRoom(room_file) that will load data into the Room object.
    OLD !
*/


class Ambient
{
public:
    // Fields
    std::vector<Animation> animations;
    // Constructor
    Ambient() {};
    Ambient(std::vector<Animation> _animations)
        : animations{_animations} {};
    // Destructor
    ~Ambient() {};
    // Methods
    void update(int dt);
};


/*
    Room has its own background texture. All ambient animations are handled
    by RoomManager.
    Room also has a click_map, which is a bitmap (.bmp) loaded into surface.
    Different active sposts are represented by different colors on the click_map.

    TO DO: Should prepare a list af aviable colours. Or handle them "dynamically"?
    mouse click -> get color from click_map -> find color in some unordered_map
    or something like that

    TO DO: split Ambient, Room ad RoomManager to separate files.
*/
class Room
{
private:
    bool visible_click_map {false};
    bool visible_walk_area {false};
public:
    // Fields - should move most stuff to be private
    Texture* texture;                                   // Backgroud texture.
    Polygon walk_area;
    PolygonObject* screen_walk_area;  // Initialized to nullptr, is that correct?
    SDL_Surface* click_map;                             // A bitmap of hot-spots.
    Texture* click_map_texture; // Initialized to nullptr, is that correct?
    std::unordered_map<Uint32, std::string> doors;      // A map of doors and their destinations.
    std::unordered_map<Uint32, std::string> actions;    // A map of hot-spots indexes and their actions.
    Ambient* ambient;                                   // Manager of ambient animations. Is it hecessary to have separate class for this?
    // Constructor - move implementation to .cpp
    Room() {};
    Room(Texture* _texture,
         Polygon _walk_area,
         SDL_Surface* _click_map,
         std::unordered_map<Uint32, std::string> _doors,
         std::unordered_map<Uint32, std::string> _actions,
         std::vector<Animation> _animations
        )
        : texture{_texture}, walk_area{_walk_area}, click_map{_click_map}, doors{_doors}, actions{_actions}
    {
        ambient = new Ambient{_animations};
        screen_walk_area = nullptr;
        click_map_texture = nullptr;
    };
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


class RoomManager
{
private:
    static RoomManager* singleton_;
    RoomManager() {};
    std::unordered_map<std::string, Room> rooms;
    Room* active_room;
    const std::string path {"D:/Prog/cpp/game_engine_architecture/res/rooms/"}; // Absolutw path for debugging purposes only!
public:
    static RoomManager* GetInstance();
    void startUp();
    void shutDown();
    void load_rooms(std::string suite_file);
    void activate_room(std::string room_name);
    void update(int dt);
    void handle_click(int x, int y);
    void handle_keyboard(std::string key);
};

#endif // ROOM_MANAGER_H