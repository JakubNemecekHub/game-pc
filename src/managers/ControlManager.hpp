#pragma once

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>

#include "../utils/Mouse.hpp"
#include "LogManager.hpp"
#include "StateManager.hpp"
#include "WindowManager.hpp"


class ControlManager
{
private:

    LogManager*    log_;
    WindowManager* window_;
    StateManager* state_;

public:

    ControlManager() {};
    ControlManager(LogManager* log, StateManager* state_,  WindowManager* window);

    void startUp() { log_->log("Control Manager started."); }
    void shutDown();

    void handle_window(SDL_Event& event);

    inline Mouse::Status mouse_transform(SDL_Event& event)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Mouse::BUTTON b { Mouse::BUTTON::NONE };
        // Get state of the buttons.
        // Not sure how exactly to do this. Mouse Button events and Mouse motion event
        // seem to require different aproaches.
        // Also, during motion event more than one button can probably be pressed!
        switch ( event.type )
        {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            switch ( event.button.button )
            {
            case SDL_BUTTON_LEFT:  b = Mouse::BUTTON::LEFT;  break;
            case SDL_BUTTON_RIGHT: b = Mouse::BUTTON::RIGHT; break;
            }
            break;
        case SDL_MOUSEMOTION:
            switch ( event.motion.state )
            {
            case SDL_BUTTON_LMASK:  b = Mouse::BUTTON::LEFT;  break;
            case SDL_BUTTON_RMASK:  b = Mouse::BUTTON::RIGHT; break;
            }
            break;
        }
        return Mouse::Status
        {
            static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel),
            b
        };
    }
    
};
