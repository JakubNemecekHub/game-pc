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
    SDL_Keycode KEY_WALK_POLYGON;
    SDL_Keycode KEY_ITEM_POLYGON;
    SDL_Keycode KEY_ITEM_VECTOR;
    SDL_Keycode KEY_HOT_SPOTS;
    SDL_Keycode KEY_EDITOR;
};


class ControlManager
{
private:

    LogManager*    log_;
    WindowManager* window_;
    StateManager* state_;

    Controls       mapping_;
    SDL_Event      event_;

public:

    ControlManager() {};
    ControlManager(LogManager* log, StateManager* state_,  WindowManager* window);

    void startUp(YAML::Node mapping);
    void shutDown();

    Controls mapping();
    
    void handle_window(SDL_Event event);
    Mouse::Transform mouse_transform(SDL_Event event);
    
};
