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
    -> walking area
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
    TextManager::GetInstance()->clean();
    active_room = &rooms[room_name];
}


void RoomManager::update(int dt)
{
    active_room->update(dt);
}


void RoomManager::handle_click(int x, int y, bool right_click)
{
    // Check walk area
    if ( active_room->point_in_polygon(x, y) )
    {
        std::cout << "I am inside." << std::endl;
        // TextManager::GetInstance()->register_text("I am inside.", x, y);
    }
    Uint32 response;
    // Check Items
    Item* clicked_item;
    clicked_item = active_room->item_get(x, y);
    if ( clicked_item != nullptr )
    {
        // Use item
        if ( right_click )  // If right click, then look -> show text
        {
            TextManager::GetInstance()->register_text(clicked_item->look(), x, y);
        }
        else                // If left click then use -> also show text
        {
            TextManager::GetInstance()->register_text(clicked_item->use(), x, y, GREEN);
            clicked_item->set_state(false);
        }
    }
    // Check Ambient objects and doors
    response = active_room->get_mapped_object(x, y);
    if ( response )
    {
        // Ambient
        AmbientObject* obj_a = active_room->get_ambient_object(response);
        if ( obj_a != nullptr )
        {
            if ( right_click )  // If right click, then look -> show text
            {
                TextManager::GetInstance()->register_text(obj_a->look(), x, y);
            }
            else                // If left click then use -> also show text
            {
                TextManager::GetInstance()->register_text(obj_a->use(), x, y, GREEN);
            }
        }
        // Door
        Door* obj_d = active_room->get_door(response);
        if ( obj_d != nullptr )
        {
            if ( right_click )  // If right click, then look -> show text
            {
                TextManager::GetInstance()->register_text(obj_d->look(), x, y, PURPLE);
            }
            else                // If left click then use -> change room
            {
                if ( obj_d->get_locked() )
                {
                    TextManager::GetInstance()->register_text(obj_d->look_locked(), x, y, PURPLE);
                }
                else
                {
                    activate_room(obj_d->get_target());
                }
            }
        }
    }
}


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
        active_room->toggle_click_map();
    }
    if ( key == "polygon" )
    {
        active_room->toggle_walk_area();
    }
}