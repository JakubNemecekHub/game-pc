#include "RoomManager.hpp"

#include "RenderManager.hpp"


// ======= Room


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
    load_room("../res/rooms/bedroom.png");
    RenderManager::GetInstance()->registerTexture(room->texture);
}

void RoomManager::load_room(const char* room_file)
{
    room->texture = RenderManager::GetInstance()->loadTexture(room_file, scale_full_h);

}