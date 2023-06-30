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


class RoomManager
{
private:

    LogManager*   log_;
    ItemManager*  items_;
    AssetManager* assets_;
    std::unordered_map<std::string, Room> rooms_;
    Room* active_room_;
    std::string path_ {"D:/Prog/game_project/game/res/"};

    void load_rooms_(std::string suite_file);

public:

    RoomManager();
    RoomManager(LogManager* log, ItemManager* items, AssetManager* assets);

    bool startUp();
    bool shutDown();

    void activate_room(const std::string& room_id);
    void update(RenderManager* renderer, int dt);
    // bool walkable(float x, float y);
    GameObject* get_object(float x, float y);
    void remove_item(std::string id);

    inline void toggle_walk_area()      { active_room_->toggle_walk_area();      }
    inline void toggle_item_debug()     { active_room_->toggle_item_debug();     }
    inline void toggle_hot_spot_debug() { active_room_->toggle_hot_spot_debug(); }
    inline void toggle_door_debug()     { active_room_->toggle_door_debug();     }

};
