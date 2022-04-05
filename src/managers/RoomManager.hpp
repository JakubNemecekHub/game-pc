#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <iostream>
// #include <array>
#include <unordered_map>
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
private:
public:
    // Fields
    Texture* texture;
    // Constructor
    Room() {};
    // Destructor
    ~Room();
    // Methods
    void handleEvents(SDL_Event event);
    void update(int dt);
    void render();
};

class Ambient
{
public:
    // Fields
    std::unordered_map<std::string, Animation> animations; // Here a vector or std::array would be better, I want to loop them
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
    const std::string path {"D:/Prog/cpp/game_engine_architecture/res/rooms/"};
public:
    static RoomManager* GetInstance();
    void startUp();
    void shutDown();
    void load_room(std::string room_file);
    void update(int dt);
};

#endif // ROOM_MANAGER_H