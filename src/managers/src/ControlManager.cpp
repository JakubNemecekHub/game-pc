#include "../ControlManager.hpp"

#include <iostream>

#include "../../Game.hpp"


ControlManager::ControlManager(LogManager* log, WindowManager* window, RoomManager* rooms, PlayerManager* player)
    : log_{log}, window_{window}, rooms_{rooms}, player_{player} {}

void ControlManager::startUp(YAML::Node mapping)
{
    log_->log("Starting Control Manager.");
    mapping_ = Controls{
        SDL_GetKeyFromName(mapping["Inventory"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Walk_Polygon"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Item_Polygon"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Item_Vector"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Hots_pots"].as<std::string>().c_str()),
    };
    screen_ = new ScreenGameplayNormal;
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


Mouse::click ControlManager::handle_mouse(SDL_Event event)
{
    if ( event.type != SDL_MOUSEBUTTONUP ) return std::make_tuple(false, 0, 0, false);
    
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool right_click;
    right_click = ( event.button.button == SDL_BUTTON_RIGHT );
    return std::make_tuple(true, x, y, right_click);
}

auto ControlManager::mouse_data(SDL_Event event)
{
    struct result { int x; int y; bool right_click; };
    int x_, y_;
    SDL_GetMouseState(&x_, &y_);
    bool right_click { event.button.button == SDL_BUTTON_RIGHT };
    return result{x_, y_, right_click};
}


Controls ControlManager::mapping() { return mapping_; }

/*****************************************************************************************
 * Screens
*****************************************************************************************/

void ControlManager::screen(SCREEN type)
{
    delete screen_;
    switch ( type )
    {
        case SCREEN::GAMEPLAY_NORMAL: screen_ = new ScreenGameplayNormal; break;
        case SCREEN::GAMEPLAY_INVENTORY: screen_ = new ScreenGameplayInventory; break;
        default: break;
    }
}

ScreenControl* ControlManager::screen() { return screen_; }

void ScreenGameplayNormal::accept_keyboard(Game* handler, SDL_Event event)
{
    handler->input_keyboard(this, event);
}

void ScreenGameplayInventory::accept_keyboard(Game* handler, SDL_Event event)
{
    handler->input_keyboard(this, event);
}

void ScreenGameplayNormal::accept_mouse(Game* handler, SDL_Event event)
{
    handler->input_mouse(this, event);
}

void ScreenGameplayInventory::accept_mouse(Game* handler, SDL_Event event)
{
    handler->input_mouse(this, event);
}