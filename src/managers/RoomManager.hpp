#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

#include "LogManager.hpp"
#include "RenderManager.hpp"
#include "ItemManager.hpp"
#include "AssetManager.hpp"
#include "../components/Room.hpp"
#include "../components/GameObject.hpp"
#include "../components/Item.hpp"
#include "../components/Door.hpp"
#include "../components/HotSpot.hpp"


enum ACTION_ROOM
{
    BITMAP,
    WALK_POLYGON,
    ITEM_POLYGON,
    ITEM_VECTOR,
};


class RoomManager
{
private:

    LogManager* log_;
    std::unordered_map<std::string, Room> rooms_;
    Room* active_room_;
    std::string path_ {"D:/Prog/game_project/game/res/"};

    void load_rooms_(std::string suite_file, ItemManager* items, AssetManager* assets);

public:

    RoomManager();
    RoomManager(LogManager* log);

    bool startUp(ItemManager* items, AssetManager* assets);
    bool shutDown();

    void activate_room(const std::string& room_id);
    void update(RenderManager* renderer, int dt);
    bool walkable(int x, int y);
    GameObject* get_object(int x, int y);
    // HotSpot* get_hot_spot(int x, int y);
    // Door* get_door(int x, int y);
    // Item* get_item(int x, int y);
    void remove_item(std::string id);
    void handle_keyboard(ACTION_ROOM action);

};
