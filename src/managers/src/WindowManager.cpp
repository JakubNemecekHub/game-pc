#include "../WindowManager.hpp"

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

void WindowManager::toggle_fullscreen()
{
    if ( !(SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN) )
    {
        // From windowed to fullscreen
        SDL_GetWindowSize(window_, &width, &height);    // Save current window dimensions
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        // From fullscreen to windowed
        SDL_SetWindowFullscreen(window_, 0);                     // set windowed
        SDL_SetWindowSize(window_, width, height); // Set original dimensions
    }
}

void WindowManager::close()
{
    isRunning = false;
}
