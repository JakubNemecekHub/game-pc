#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>

#include "../utils/Mouse.hpp"
#include "LogManager.hpp"
#include "StateManager.hpp"
#include "WindowManager.hpp"


struct Controls
{
    SDL_Keycode KEY_INVENTORY;
    SDL_Keycode KEY_EDITOR;
    SDL_Keycode KEY_EDITOR_ITEMS;
    SDL_Keycode KEY_EDITOR_HOT_SPOTS;
    SDL_Keycode KEY_EDITOR_DOORS;
    SDL_Keycode KEY_EDITOR_WALK_POLYGON;
    SDL_Keycode KEY_EDITOR_BITMAP;
};


class ControlManager
{
private:

    LogManager*    log_;
    WindowManager* window_;
    StateManager* state_;

    Controls       mapping_;

public:

    ControlManager() {};
    ControlManager(LogManager* log, StateManager* state_,  WindowManager* window);

    void startUp(YAML::Node mapping);
    void shutDown();

    Controls mapping();
    
    void handle_window(SDL_Event& event);
    auto mouse_position(SDL_Event& event)
    {
        struct result{ int x; int y; };
        int x, y;
        SDL_GetMouseState(&x, &y);
        return result{x, y};
    }
    
};
