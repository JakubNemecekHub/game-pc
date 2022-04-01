#include "WindowManager.hpp"

#include <iostream>

WindowManager* WindowManager::singleton_ = nullptr;

WindowManager* WindowManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new WindowManager();
    }
    return singleton_;
}

void WindowManager::startUp()
{
    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
    if ( window_ )
    {
        isRunning = true;
    }
}

void WindowManager::shutDown()
{
}

// void WindowManager::handleEvents()
// {
//     while ( SDL_PollEvent(&event) )
//     {
//         if ( event.type == SDL_QUIT)
//             isRunning = false;
//     }
// }

SDL_Window* WindowManager::window()
{
    return window_;
}

void WindowManager::close()
{
    isRunning = false;
}
