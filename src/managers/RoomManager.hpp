#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <iostream>

#include <SDL2/SDL.h>

#include "../components/Texture.hpp"

/*
    When starting up RoomManager, create empty Room (just a field that will hold a pointer to a Room).
    Then implement method LoadRoom(room_file) that will load data into the Rom object.
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

class RoomManager
{
private:
    static RoomManager* singleton_;
    RoomManager() {};
    Room* room;
public:
    static RoomManager* GetInstance();
    void startUp();
    void shutDown();
    void load_room(const char* room_file);
};


#endif // ROOM_MANAGER_H