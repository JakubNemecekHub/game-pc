#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // unique_ptr
#include <queue>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "../../math/Polygon.hpp"

namespace fs = std::filesystem;


RoomManager::RoomManager()
    : active_room_{nullptr} {}


RoomManager::RoomManager(LogManager* log)
    : log_{log} {}


bool RoomManager::startUp(ItemManager* items, AssetManager* assets)
{
    log_->log("Starting Room Manager.");
    load_rooms_("rooms", items, assets);
    activate_room("hall");
    return true;
}


bool RoomManager::shutDown()
{
    log_->log("Shutting down Room Manager.");
    active_room_ = nullptr;
    return true;
}


/*
    Load necessary room data. All information is stored in a .yaml file.

*/
void RoomManager::load_rooms_(std::string suite_file, ItemManager* items, AssetManager* assets)
{
    log_->log("Loading rooms.");
    std::queue<fs::directory_entry> rooms_meta { assets->rooms_meta() };
    while ( !rooms_meta.empty() )
    {
        fs::directory_entry entry { rooms_meta.front() };
        YAML::Node room_data      { YAML::LoadFile(entry.path().string()) };
        std::string id            { room_data["id"].as<std::string>() };
        rooms_.emplace(std::piecewise_construct,
                      std::forward_as_tuple(id),
                      std::forward_as_tuple(room_data, items, assets));
        log_->log("Room \"", id, "\" created.");
        rooms_meta.pop();
    }
}


void RoomManager::activate_room(const std::string& id)
{
    try
    {
        active_room_ = &rooms_.at(id);
    }
    catch(const std::out_of_range& e)
    {
        log_->error("Cannot activate missing room \"", id, "\"");
    }
    
}


void RoomManager::update(RenderManager* renderer, int dt)
{
    active_room_->update(renderer, dt);
}


bool RoomManager::walkable(int x, int y) { return active_room_->walkable(x, y); }
GameObject* RoomManager::get_object(int x, int y) { return active_room_->get_object(x, y); }
// HotSpot* RoomManager::get_hot_spot(int x, int y) { return active_room_->get_hot_spot(x, y); }
// Door* RoomManager::get_door(int x, int y) { return active_room_->get_door(x, y); }
// Item* RoomManager::get_item(int x, int y) { return active_room_->get_item(x, y); }
void RoomManager::remove_item(std::string id) { active_room_->remove_item(id); }


/*
    Handle keyboard inputs concerning room an room management.
*/
void RoomManager::handle_keyboard(ACTION_ROOM action)
{
    switch ( action )
    {
    case ACTION_ROOM::BITMAP:
        active_room_->toggle_click_map();
        break;
    case ACTION_ROOM::WALK_POLYGON:
        active_room_->toggle_walk_area();
        break;
    case ACTION_ROOM::ITEM_POLYGON:
        active_room_->toggle_item_click_map();
        break;
    case ACTION_ROOM::ITEM_VECTOR:
        active_room_->toggle_item_vector();
        break;
    default:
        break;
    }
}