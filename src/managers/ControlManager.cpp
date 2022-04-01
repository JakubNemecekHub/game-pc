#include "ControlManager.hpp"

#include "WindowManager.hpp"

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
        // Mouse click
        if ( event.type == SDL_MOUSEBUTTONUP )
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            bool left;
            left = ( event.button.button == SDL_BUTTON_LEFT );
            std::cout << left << " ";
            std::cout << x << ", " << y << std::endl;
        }
    }
}