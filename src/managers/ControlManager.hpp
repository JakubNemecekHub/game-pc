#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>

#include "../utils/Mouse.hpp"
#include "LogManager.hpp"
#include "WindowManager.hpp"


struct Controls
{
    SDL_Keycode KEY_INVENTORY;
    SDL_Keycode KEY_WALK_POLYGON;
    SDL_Keycode KEY_ITEM_POLYGON;
    SDL_Keycode KEY_ITEM_VECTOR;
    SDL_Keycode KEY_HOT_SPOTS;
};


class ControlManager
{
private:

    LogManager*    log_;
    WindowManager* window_;

    Controls       mapping_;
    SDL_Event      event_;

public:

    ControlManager() {};
    ControlManager(LogManager* log, WindowManager* window);

    void startUp(YAML::Node mapping);
    void shutDown();

    Controls mapping();
    
    void handle_window(SDL_Event event);
    Mouse::click handle_mouse(SDL_Event event);
    auto mouse_data(SDL_Event event);
    
};
