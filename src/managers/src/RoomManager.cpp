#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

#include "../RenderManager.hpp"
#include "../../components/Animation.hpp"


/************************************************************************
    Room
*************************************************************************/

/*
    Room should take up the whole screen height - > set scale_full_h argument to true while loading texture.
    It should also be aligned to the middle of the screen. Important when texture width less that screen width. TO DO.
    Rooms wider than screen size should sroll. TO DO.
    <- Room must keep track of its destination (position and dimensions) on its own. Maybe this is true for all objects.
*/
Room::Room()
{
}

Room::~Room()
{
    texture = new Texture;
}


void Room::update(int dt)
{
    RenderManager::GetInstance()->register_room_texture(texture);
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
    room = new Room;
    ambient = new Ambient;
    load_room("summer-room");
}


void RoomManager::shutDown()
{
    delete ambient;
    delete room;
}


/*
    Load necessary room data. All information is stored in a .room file.
    This include:
        -> texture
        -> ambient animation
        also
        -> texture size (probably do not need, RenderManager can get it from the
                            texture itself)
        -> wlaking area (some kind of polygon, need to investigate)
        -> items present in the room
        -> general active spots
*/
// void RoomManager::load_room_XXX(std::string room_file)
// {
//     // Load Room background texture
//     std::string full_texture_path {path + room_file + ".png"};
//     room->texture = RenderManager::GetInstance()->load_texture(full_texture_path);
//     // Fit to screen height.
//     RenderManager::GetInstance()->scale_full_h(room->texture);
//     // Set default position.
//     int x;
//     x = (RenderManager::GetInstance()->get_screen_width() - room->texture->dest_rect.w) / 2;
//     room->texture->set_position(x, 0);
//     // Load click map -> move to RenderManager
//     std::string click_map_file_path {path + room_file + "-map.bmp"};
//     room->click_map = SDL_LoadBMP(click_map_file_path.c_str());
//     if ( room->click_map == NULL )
//         std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
//     SDL_LockSurface(room->click_map); // So that I can read pixels
//     // Load ambient animations
//     std::string full_anim_path {path + room_file + ".anim"};
//     ambient->animations = Animation::load_animation(full_anim_path);
// }

void RoomManager::load_room(std::string room_file)
{
    /*
        Plan of action:
        > Load json file with all the information
        > Load room background texture (in Room, Texture* texture)
        > Load room hot-spots map (in Room, SDL_Surface click_map)
        > Load room actions (in Room, std::unordered_map<Uint32, std::string> actions)
        > load room ambient animations (in object Ambient, std::unordered_map<std::string, Animation> animations)
    */

    // Open json file
    json room_data;
    std::ifstream animation_json(path + room_file + ".json");
    animation_json >> room_data;
    // Load room background texture
    room->texture = RenderManager::GetInstance()->load_texture(room_data["texture"]);
    // Fit to screen height.
    RenderManager::GetInstance()->scale_full_h(room->texture);
    // Set default position.
    int x;
    x = (RenderManager::GetInstance()->get_screen_width() - room->texture->dest_rect.w) / 2;
    room->texture->set_position(x, 0);
    // Load room hot-spots map
    room->click_map = RenderManager::GetInstance()->load_bitmap(room_data["map"]);
    // Load room hot-spots map
    for ( auto &action : room_data["actions"] )
    {
        room->actions.insert({{action[0], action[1]}});
    }
    // load room ambient animations
    ambient->animations = Animation::load_animation_vector(room_data["ambient"]);
}


void RoomManager::update(int dt)
{
    room->update(dt);
    ambient->update(dt);
}


void RoomManager::handle_click(int x, int y)
{
    Uint32 response;
    response = room->get_mapped_object(x, y);
    std::cout << response << " >> " << room->actions[response] << std::endl;
}


void RoomManager::handle_keyboard(std::string key)
{
    // Use enum?
    if ( key == "show" )
    {
        room->render_click_map();
    }
    if ( key == "hide" )
    {
        room->hide_click_map();
    }

}