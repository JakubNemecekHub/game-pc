#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // unique_ptr

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../../LogManager.hpp"
#include "../../RenderManager.hpp"
#include "../../TextManager.hpp"
#include "../../../components/Animation.hpp"
#include "../../../math_objects/PolygonObject.hpp"
#include "../../../math/Polygon.hpp"


RoomManager* RoomManager::singleton_ = nullptr;


RoomManager* RoomManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new RoomManager();
    }
    return singleton_;
}


void RoomManager::startUp()
{
    LogManager::GetInstance()->log_message("Starting Room Manager.");
    active_room = nullptr;
    load_rooms("suite-house");
    activate_room("Bedroom");
}


void RoomManager::shutDown()
{
    LogManager::GetInstance()->log_message("Shutting down Room Manager.");
    active_room = nullptr;
}


/*
    Load necessary room data. All information is stored in a .json file.
    This include:
    -> room background texture
    -> room hot-spots map
    -> room actions
    -> room ambient animations
    -> wlaking area
    TO DO: add
    -> items present in the room
*/
void RoomManager::load_rooms(std::string suite_file)
{

    // Open json file
    json suite_data;
    std::ifstream animation_json(path + suite_file + ".json");
    animation_json >> suite_data;

    for ( auto &room_data : suite_data["rooms"] )
    {
        /*
            Create empty Room in the rooms map and the use its load method.
        */
        std::string _name;
        _name = room_data["name"];
        rooms.emplace(std::piecewise_construct,
                      std::forward_as_tuple(_name),
                      std::forward_as_tuple());
        rooms.at(_name).load(room_data);
    }
}


void RoomManager::activate_room(std::string room_name)
{
    active_room = &rooms[room_name];
}


void RoomManager::update(int dt)
{
    active_room->update(dt);
}


void RoomManager::handle_click(int x, int y, bool right_click)
{
    // Convert viewport coordinates into world coordinates
    int world_x, world_y;
    world_x = static_cast<int>((x - active_room->texture->dest_rect.x) / active_room->texture->scale);
    world_y = static_cast<int>(y / active_room->texture->scale);
    std::cout << "Screen coordinates: (" << x << ", " << y << "), ";
    std::cout << "World coordinates: (" << world_x << ", " << world_y << ")" << std::endl;
    // Check walk area
    if ( active_room->walk_area.point_in_polygon(world_x, world_y) )
    {
        std::cout << "I am inside." << std::endl;
    }
    // Print out action
    Uint32 response;
    response = active_room->get_mapped_object(world_x, world_y);
    if ( response )
    {
        // If right click, then do look. e.g. do action
        if ( right_click )
        {
            TextManager::GetInstance()->register_text(active_room->actions[response], x, y);
        }
        // If left click then use, e.g. use door
        else
        {
            if ( active_room->doors.find(response) != active_room->doors.end() )
            {
                // Change room
                std::cout << "Changing room to " << active_room->doors[response] << std::endl;
                activate_room(active_room->doors[response]);
            }
        }
    }
}


/*
    Handle keyboars inputs concerning room an room management.
    b: Toggle renderinf of click map.
    p: Toggle rendreing of walk area.
*/
void RoomManager::handle_keyboard(std::string key)
{
    // Use enum?
    if ( key == "bitmap" )
    {
        active_room->toggle_click_map();
    }
    if ( key == "polygon" )
    {
        active_room->toggle_walk_area();
    }
}