#include "../RoomManager.hpp"

#include <iostream>
#include <string>

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

Room::~Room()
{
    texture = new Texture;
}

void Room::update(int dt)
{
    RenderManager::GetInstance()->registerTexture(texture);
}

void Room::render()
{
}


/************************************************************************
    Ambient
*************************************************************************/
void Ambient::update(int dt)
{
    /*
        Update each ambient animation and register it to the renderer
    */
   for ( auto &animation : animations )
   {
       animation.second.update(dt);
       RenderManager::GetInstance()->registerTexture(animation.second.texture);
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
    load_room("front-gate");
}

void RoomManager::shutDown()
{
    delete ambient;
    delete room;
}

void RoomManager::load_room(std::string room_file)
{
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
    // Load Room background texture
    std::string full_texture_path {path + room_file + ".png"};
    room->texture = RenderManager::GetInstance()->load_texture(full_texture_path);
    // Fit to screen height.
    RenderManager::GetInstance()->scale_full_h(room->texture);
    // Set default position.
    int x;
    x = (RenderManager::GetInstance()->get_screen_width() - room->texture->dest_rect.w) / 2;
    room->texture->set_position(x, 0);
    // RenderManager::GetInstance()->registerTexture(room->texture);
    // // Load ambient animations
    std::string full_anim_path {path + room_file + ".anim"};
    ambient->animations = Animation::load_animation(full_anim_path);
}

void RoomManager::update(int dt)
{
    room->update(dt);
    ambient->update(dt);
}