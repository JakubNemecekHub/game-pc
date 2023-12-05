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


RoomManager::RoomManager(LogManager* log, ItemManager* items, AssetManager* assets, SerializationManager* io)
    : log_{log}, items_{items}, assets_{assets}, io_{io} {}


bool RoomManager::startUp(std::string id)
{
    log_->log("Starting Room Manager.");
    load_rooms_();
    activate_room(id);
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
void RoomManager::load_rooms_()
{
    log_->log("Loading rooms.");
    std::queue<fs::directory_entry> rooms_meta { assets_->rooms_meta() };
    while ( !rooms_meta.empty() )
    {
        fs::directory_entry entry { rooms_meta.front() };
        try
        {
            YAML::Node room_data { YAML::LoadFile(entry.path().string()) };
            std::string id       { room_data["id"].as<std::string>() };
            rooms_.emplace(std::piecewise_construct,
                        std::forward_as_tuple(id),
                        std::forward_as_tuple(room_data, items_, assets_));
            log_->log("Room \"", id, "\" created.");
        }
        catch(const YAML::ParserException& e)
        {
            log_->error("Cannot parse file", entry.path().string(), ", Reason: ", e.msg);
        }
        rooms_meta.pop();
    }
    log_->log("Rooms loaded.");
}


void RoomManager::activate_room(const std::string& id)
{  
    if (auto it = rooms_.find(id); it != rooms_.end()) active_room_ = &(it->second);
    else log_->error("Cannot activate missing room \"", id, "\"");
}


void RoomManager::update(RenderManager* renderer, int dt)
{
    active_room_->update(renderer, dt);
}


bool RoomManager::walkable(float x, float y) { return active_room_->walkable(x, y); }
GameObject* RoomManager::get_object(float x, float y) { return active_room_->get_object(x, y); }
void RoomManager::remove_item(std::string id) { active_room_->remove_item(id); }


void RoomManager::save()
{
    if ( !io_->open_out() ) return;  // TO DO: return false and do something with it?

    // TO DO: Save name of the active Room.
    std::ranges::for_each(rooms_, [&](auto& pair)
    {
        io_->write(pair.first); // Room id
        pair.second.save(io_);  // Write what is necessary in Room
        io_->write("\n");       // New line, for convenience
    });
    io_->close_out();
}