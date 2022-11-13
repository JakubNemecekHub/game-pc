#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

#include "Room.hpp"


class RoomManager
{
private:
    static RoomManager* singleton_;
    RoomManager() {};
    std::unordered_map<std::string, Room> rooms;
    Room* active_room;
    const std::string path {"D:/Prog/game_project/game/res/rooms/"}; // Absolute path for debugging purposes only!
public:
    static RoomManager* GetInstance();
    void startUp();
    void shutDown();
    void load_rooms(std::string suite_file);
    void activate_room(std::string room_name);
    void update(int dt);
    void handle_click(int x, int y, bool right_click);
    void handle_keyboard(std::string key);
};
