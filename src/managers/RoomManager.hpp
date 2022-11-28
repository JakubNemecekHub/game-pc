#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

#include "LogManager.hpp"
#include "RenderManager.hpp"
#include "ItemManager.hpp"
#include "TextureManager.hpp"
#include "../components/Room.hpp"
#include "../components/Item.hpp"
#include "../components/Door.hpp"
#include "../components/HotSpot.hpp"

class RoomManager
{
private:

    LogManager* log_;
    std::unordered_map<std::string, Room> rooms_;
    Room* active_room_;
    std::string path_ {"D:/Prog/game_project/game/res/"};

    void load_rooms_(std::string suite_file, RenderManager* renderer, ItemManager* items, TextureManager* textures);

public:

    RoomManager();
    RoomManager(LogManager* log);

    bool startUp(RenderManager* renderer, ItemManager* items, TextureManager* textures);
    bool shutDown();

    void activate_room(std::string room_name);
    void update(RenderManager* renderer, int dt);
    bool walkable(int x, int y);
    HotSpot* get_hot_spot(int x, int y);
    Door* get_door(int x, int y);
    Item* get_item(int x, int y);
    void remove_item(std::string id);
    void handle_keyboard(std::string key);

};
