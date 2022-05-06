#include "../ControlManager.hpp"

#include "../LogManager.hpp"
#include "../RoomManager.hpp"
#include "../WindowManager.hpp"

#include <iostream>


ControlManager* ControlManager::singleton_ = nullptr;


ControlManager* ControlManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new ControlManager();
    }
    return singleton_;
}


void ControlManager::startUp()
{
    LogManager::GetInstance()->log_message("Starting Control Manager.");
}


void ControlManager::shutDown()
{
    LogManager::GetInstance()->log_message("Shutting down Control Manager.");
}


void ControlManager::handleEvents()
{
    while ( SDL_PollEvent(&event) )
    {
        // Close Window
        if ( event.type == SDL_QUIT)
            WindowManager::GetInstance()->close();
        if ( event.key.keysym.sym == SDLK_ESCAPE )
            WindowManager::GetInstance()->close();
        // Fullscreen
        if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_F11 )  // Should only catch one F11 event
        {
            WindowManager::GetInstance()->toggle_fullscreen();
        }
        // Mouse click
        if ( event.type == SDL_MOUSEBUTTONUP )
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            bool left;
            left = ( event.button.button == SDL_BUTTON_LEFT );
            // Pass mouse click along
            RoomManager::GetInstance()->handle_click(x, y);
        }
        // Toggle Static texture
        if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_b )
        {
            RoomManager::GetInstance()->handle_keyboard("bitmap");
        }
        // Toggle polygon rendering
        if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_p )
        {
            RoomManager::GetInstance()->handle_keyboard("polygon");
        }
    }
}