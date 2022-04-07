#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

#include "../RenderManager.hpp"
#include "../../components/Animation.hpp"


/************************************************************************
    Ambient
*************************************************************************/

/*
    Update each ambient animation and register it to the renderer
*/
void Ambient::update(int dt)
{
   for ( auto &animation : animations )
   {
       animation.update(dt);
       RenderManager::GetInstance()->register_ambient_texture(animation.texture);
   }
}


/************************************************************************
    Room
*************************************************************************/

/*
    Room should take up the whole screen height - > set scale_full_h argument to true while loading texture.
    It should also be aligned to the middle of the screen. Important when texture width less that screen width. TO DO.
    Rooms wider than screen size should sroll. TO DO.
    <- Room must keep track of its destination (position and dimensions) on its own. Maybe this is true for all objects.
*/
Room::~Room()
{
    delete texture;
    delete click_map;
}


void Room::update(int dt)
{
    RenderManager::GetInstance()->register_room_texture(texture);
    ambient->update(dt);
}


void Room::render_click_map()
{
    RenderManager::GetInstance()->register_static_surface(click_map);
}


void Room::hide_click_map()
{
    RenderManager::GetInstance()->delist_static_surface();
}


Uint32 Room::get_mapped_object(int x, int y)
{
    // First need to convert x, y to source image coordinates
    // that means scaling it and also moving x coordinates
    int _x, _y;
    _x = (x - texture->dest_rect.x) / texture->scale;
    _y = y / texture->scale;
    int bpp = click_map->format->BytesPerPixel; // bytes per pixel, depeds on loaded image
    Uint8 *p = (Uint8 *)click_map->pixels + _y * click_map->pitch + _x * bpp;
    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                return p[0] << 16 | p[1] << 8 | p[2];
            }
            else
            {
                return p[0] | p[1] << 8 | p[2] << 16;
            }
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
    }
}


/************************************************************************
    Room Manager
*************************************************************************/
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
    // room = new Room;
    // ambient = new Ambient;
    active_room = nullptr;
    load_rooms("suite-house");
    activate_room("Bedroom");
}


void RoomManager::shutDown()
{
    // delete ambient;
    // delete room;
}


/*
    Load necessary room data. All information is stored in a .json file.
    This include:
    -> room background texture
    -> room hot-spots map
    -> room actions
    -> room ambient animations
    TO DO: add
    -> wlaking area (some kind of polygon, need to investigate)
    -> items present in the room
*/
void RoomManager::load_rooms(std::string suite_file)
{

    // std::unordered_map<std::string, Room> rooms;
    // Open json file
    json suite_data;
    std::ifstream animation_json(path + suite_file + ".json");
    animation_json >> suite_data;

    for ( auto &room_data : suite_data["rooms"] )
    {
        /*
            Potřebuji jiný přístup:
            Sesbírat informace a pak emplace novou místnost do unordered_map
        */

        // Load room name
        std::string _name;
        _name = room_data["name"];
        // Load room background texture
        Texture* _texture;
        _texture = RenderManager::GetInstance()->load_texture(room_data["texture"]);
        // Fit to screen height.
        RenderManager::GetInstance()->scale_full_h(_texture);
        // Set default position.
        int x;
        x = (RenderManager::GetInstance()->get_screen_width() - _texture->dest_rect.w) / 2;
        _texture->set_position(x, 0);
        // Load room hot-spots map
        SDL_Surface* _click_map;
        _click_map = RenderManager::GetInstance()->load_bitmap(room_data["map"]);
        // Load room hot-spots map
        std::unordered_map<Uint32, std::string> _actions;
        for ( auto &action : room_data["actions"] )
        {
            _actions.insert({{action[0], action[1]}});
        }
        // load room ambient animations
        std::vector<Animation> _animations;
        _animations = Animation::load_animation_vector(room_data["ambient"]);
        // Emplace new Room Object
        rooms.emplace(std::piecewise_construct,
                      std::forward_as_tuple(_name),
                      std::forward_as_tuple(_texture, _click_map, _actions, _animations));


    }

    // // Load room background texture
    // room->texture = RenderManager::GetInstance()->load_texture(room_data["texture"]);
    // // Fit to screen height.
    // RenderManager::GetInstance()->scale_full_h(room->texture);
    // // Set default position.
    // int x;
    // x = (RenderManager::GetInstance()->get_screen_width() - room->texture->dest_rect.w) / 2;
    // room->texture->set_position(x, 0);
    // // Load room hot-spots map
    // room->click_map = RenderManager::GetInstance()->load_bitmap(room_data["map"]);
    // // Load room hot-spots map
    // for ( auto &action : room_data["actions"] )
    // {
    //     room->actions.insert({{action[0], action[1]}});
    // }
    // // load room ambient animations
    // ambient->animations = Animation::load_animation_vector(room_data["ambient"]);
}


void RoomManager::activate_room(std::string room_name)
{
    active_room = &rooms[room_name];
}


void RoomManager::update(int dt)
{
    active_room->update(dt);
    // ambient->update(dt);
}


void RoomManager::handle_click(int x, int y)
{
    Uint32 response;
    response = active_room->get_mapped_object(x, y);
    std::cout << response << " >> " << active_room->actions[response] << std::endl;
}


void RoomManager::handle_keyboard(std::string key)
{
    // Use enum?
    if ( key == "show" )
    {
        active_room->render_click_map();
    }
    if ( key == "hide" )
    {
        active_room->hide_click_map();
    }

}