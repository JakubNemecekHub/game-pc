#include "../WindowManager.hpp"

#include <iostream>

#include "../LogManager.hpp"

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
    LogManager::GetInstance()->log_message("Starting Window Manager.");
    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
    if ( window_ )
    {
        LogManager::GetInstance()->log_message("Window created.");
        isRunning = true;
    }
    else
    {
        LogManager::GetInstance()->log_error("SDL Error: ", SDL_GetError());
    }
}

void WindowManager::shutDown()
{
    SDL_DestroyWindow(window_);
    LogManager::GetInstance()->log_message("Window destroyed. Shutting down Window Manager.");
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
