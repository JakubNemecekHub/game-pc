#include "../ControlManager.hpp"

#include <iostream>

#define kkey event.key.keysym.sym // because i don't understand why this doesn'z work: SDL_KeyCode key = event.key.keysym.sym;


ControlManager::ControlManager(LogManager* log, WindowManager* window, RoomManager* rooms, PlayerManager* player)
    : log_{log}, window_{window}, rooms_{rooms}, player_{player} {}

void ControlManager::startUp(YAML::Node mapping)
{
    log_->log("Starting Control Manager.");
    mapping_ = Controls{
        mapping["Inventory"].as<int>(),
        mapping["Walk_Polygon"].as<int>(),
        mapping["Item_Polygon"].as<int>(),
        mapping["Item_Vector"].as<int>(),
        mapping["Hots_pots"].as<int>()
    };
    log_->log("Control Manager started.");
}


void ControlManager::shutDown()
{
    log_->log("Shutting down Control Manager.");
}


void ControlManager::handle_window(SDL_Event event)
{
    if ( event.type == SDL_QUIT) window_->close();
    
}

void ControlManager::handle_keyboard(SDL_Event event)
{
    if ( event.type == SDL_KEYUP )
    {
            if ( kkey == SDLK_ESCAPE ) window_->close();
        // Fullscreen
        else if ( kkey == SDLK_F11 ) window_->toggle_fullscreen();
        // Show click map
        else if ( kkey == mapping_.KEY_HOT_SPOTS ) rooms_->handle_keyboard(ACTION_ROOM::BITMAP);
        // Toggle walk area's polygon rendering
        else if ( kkey == mapping_.KEY_WALK_POLYGON ) rooms_->handle_keyboard(ACTION_ROOM::WALK_POLYGON);
        // Toggle items' click area's polygon rendering
        else if ( kkey == mapping_.KEY_ITEM_POLYGON ) rooms_->handle_keyboard(ACTION_ROOM::ITEM_POLYGON);
        // Toggle items' position vector rendering
        else if ( kkey == mapping_.KEY_ITEM_VECTOR ) rooms_->handle_keyboard(ACTION_ROOM::ITEM_VECTOR);
        // Show Inventory
        else if ( kkey == mapping_.KEY_INVENTORY ) player_->inventory.print(log_);
    }
}

mouse_click ControlManager::handle_mouse(SDL_Event event)
{
    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        bool right_click;
        right_click = ( event.button.button == SDL_BUTTON_RIGHT );
        // Return mouse click
        return std::make_tuple(true, x, y, right_click);
    }
    return std::make_tuple(false, 0, 0, false);
}