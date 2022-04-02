#include <iostream>

#include "RoomManager.hpp"

#include "RenderManager.hpp"


// ======= Room

/*
Room should take up the whole screen height - > set scale_full_h argument to true while loading texture.
It should also be aligned to the middle of the screen. Important when texture width less that screen width. TO DO.
Rooms wider than screen size should sroll. TO DO.
<- Room must keep track of its destination (position and dimensions) on its own. Maybe this is true for all objects.
*/


// SDL_Rect src_rect = {0, 0, 105, 60};
//     SDL_Rect dest_rect = {0, 0, 1260, 720};
//     SDL_RenderCopyEx(Game::renderer, texture, &src_rect, &dest_rect, NULL, NULL, SDL_FLIP_NONE);

// Destructor
Room::~Room()
{

}

// ======= RoomManager
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
    // Create a room and send it to Renderer
    load_room("../res/rooms/hall.png");
    RenderManager::GetInstance()->registerTexture(room->texture);
}

void RoomManager::load_room(const char* room_file)
{
    room->texture = RenderManager::GetInstance()->loadTexture(room_file);
    // Fit to screen height.
    RenderManager::GetInstance()->scale_full_h(room->texture);
    // Set default position.
    int x;
    x = (RenderManager::GetInstance()->get_screen_width() - room->texture->dest_rect.w) / 2;
    room->texture->set_position(x, 0);
}