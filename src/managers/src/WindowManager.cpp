#include "../WindowManager.hpp"


WindowManager::WindowManager(LogManager* log, YAML::Node ini)
{
    log_ = log;
    title_ = ini["title"].as<std::string>();
    width_ = ini["width"].as<int>();
    height_ = ini["height"].as<int>();
    fullscreen_initial_ = ini["fullscreen"].as<bool>();
}


bool WindowManager::startUp()
{
    log_->log("Starting Window Manager.");
    window_ = SDL_CreateWindow(title_.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width_,
                               height_,
                               fullscreen_initial_);
    if ( window_ )
    {
        log_->log("Window created.");
        running = true;
        return true;
    }
    else
    {
        log_->error("SDL Error: ", SDL_GetError());
        return false;
    }
}


bool WindowManager::shutDown()
{
    SDL_DestroyWindow(window_);
    log_->log("Window destroyed. Shutting down Window Manager.");
    return true;
}


SDL_Window* WindowManager::window() { return window_; }


void WindowManager::toggle_fullscreen()
{
    if ( !(SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN) )
    {
        // From windowed to fullscreen
        SDL_GetWindowSize(window_, &width_, &height_);    // Save current window dimensions
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        // From fullscreen to windowed
        SDL_SetWindowFullscreen(window_, 0);            // set windowed
        SDL_SetWindowSize(window_, width_, height_);    // Set original dimensions
    }
}


void WindowManager::close() { running = false; }
