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

/*
    When starting up RoomManager, create empty Room (just a field that will hold a pointer to a Room).
    Then implement method LoadRoom(room_file) that will load data into the Room object.
*/

class Room
{
    /*
        Room has its own background texture. All ambient animations are handled
        by RoomManager.
        Room also has a click_map, which is a bitmap (.bmp) loaded into surface.
        Different active sposts are represented by different colors on the click_map.

        TO DO: Should prepare a list af aviable colours. Or handle them "dynamically"?
        mouse click -> get color from click_map -> find color in some unordered_map
        or something like that

        TO DO: Render click_map over the room texture. Just for testing purposes.
    */
private:
public:
    // Fields
    Texture* texture;                                       // Backgroud texture
    SDL_Surface* click_map;                                 // A bitmap of hot-spots
    std::unordered_map<Uint32, std::string> actions;        // A map of hot-spots indexes and their actions
    // Constructor
    Room();
    // Destructor
    ~Room();

    // Methods

    void handleEvents(SDL_Event event);
    void update(int dt);
    void render_click_map();
    void hide_click_map();
    Uint32 get_mapped_object(int x, int y);

};

class Ambient
{
public:
    // Fields
    // std::unordered_map<std::string, Animation> animations; // Here a vector or std::array would be better, I want to loop them
    std::vector<Animation> animations;
    // Constructor
    Ambient() {};
    // Destructor
    ~Ambient() {};
    // Methods
    void update(int dt);
};

class RoomManager
{
private:
    static RoomManager* singleton_;
    RoomManager() {};
    Room* room;
    Ambient* ambient;
    const std::string path {"D:/Prog/cpp/game_engine_architecture/res/rooms/"}; // Absolut path for debugging purposes only!
public:
    static RoomManager* GetInstance();
    void startUp();
    void shutDown();
    void load_room(std::string room_file);
    void update(int dt);
    void handle_click(int x, int y);
    void handle_keyboard(std::string key);
};

#endif // ROOM_MANAGER_H