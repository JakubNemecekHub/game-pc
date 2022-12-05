#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // unique_ptr

#include <yaml-cpp/yaml.h>

#include "../../components/Animation.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"


RoomManager::RoomManager()
    : active_room_{nullptr} {}


RoomManager::RoomManager(LogManager* log)
    : log_{log} {}


bool RoomManager::startUp(RenderManager* renderer, ItemManager* items, AssetManager* assets)
{
    log_->log("Starting Room Manager.");
    load_rooms_("rooms", renderer, items, assets);
    activate_room("Bedroom");
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
void RoomManager::load_rooms_(std::string suite_file, RenderManager* renderer, ItemManager* items, AssetManager* assets)
{

    // Open yaml file
    YAML::Node data = YAML::LoadFile(path_ + suite_file + ".yaml");

    for ( auto &room_data : data["rooms"] )
    {
        /*
            Create empty Room in the rooms map and the use its load method.
        */
        std::string name { room_data["name"].as<std::string>() };
        rooms_.emplace(std::piecewise_construct,
                      std::forward_as_tuple(name),
                      std::forward_as_tuple(room_data, renderer, items, assets));
    }
}


void RoomManager::activate_room(std::string room_name)
{
    active_room_ = &rooms_[room_name];
}


void RoomManager::update(RenderManager* renderer, int dt)
{
    active_room_->update(renderer, dt);
}


bool RoomManager::walkable(int x, int y) { return active_room_->walkable(x, y); }
HotSpot* RoomManager::get_hot_spot(int x, int y) { return active_room_->get_hot_spot(x, y); }
Door* RoomManager::get_door(int x, int y) { return active_room_->get_door(x, y); }
Item* RoomManager::get_item(int x, int y) { return active_room_->get_item(x, y); }
void RoomManager::remove_item(std::string id) { active_room_->remove_item(id); }


/*
    Handle keyboard inputs concerning room an room management.
    b: Toggle rendering of click map.
    p: Toggle rendering of walk area.
*/
void RoomManager::handle_keyboard(std::string key)
{
    // Use enum?
    if ( key == "bitmap" )
    {
        active_room_->toggle_click_map();
    }
    if ( key == "polygon" )
    {
        active_room_->toggle_walk_area();
    }
}