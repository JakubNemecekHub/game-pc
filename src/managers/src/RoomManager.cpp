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


RoomManager::RoomManager(LogManager* log, ItemManager* items, AssetManager* assets)
    : log_{log}, items_{items}, assets_{assets} {}


bool RoomManager::startUp()
{
    log_->log("Starting Room Manager.");
    load_rooms_("rooms");
    activate_room("hall");
    log_->log("Room Manager Started.");
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
void RoomManager::load_rooms_(std::string suite_file)
{
    log_->log("Loading rooms.");
    std::queue<fs::directory_entry> rooms_meta { assets_->rooms_meta() };
    while ( !rooms_meta.empty() )
    {
        fs::directory_entry entry { rooms_meta.front() };
        YAML::Node room_data      { YAML::LoadFile(entry.path().string()) };
        std::string id            { room_data["id"].as<std::string>() };
        rooms_.emplace(std::piecewise_construct,
                      std::forward_as_tuple(id),
                      std::forward_as_tuple(room_data, items_, assets_));
        log_->log("Room \"", id, "\" created.");
        rooms_meta.pop();
    }
    log_->log("Rooms loaded.");
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


bool RoomManager::walkable(float x, float y) { return active_room_->walkable(x, y); }
GameObject* RoomManager::get_object(float x, float y) { return active_room_->get_object(x, y); }
GameObject* RoomManager::get_item(float x, float y) { return active_room_->get_item(x, y); }
GameObject* RoomManager::get_hot_spot(float x, float y) { return active_room_->get_hot_spot(x, y); }
void RoomManager::remove_item(std::string id) { active_room_->remove_item(id); }