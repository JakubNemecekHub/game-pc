#pragma once

#include <iostream>
#include <tuple>

#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "RoomManager.hpp"
#include "WindowManager.hpp"
#include "PlayerManager.hpp"

#define mouse_click std::tuple<bool, int, int, bool>

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
    RoomManager*   rooms_;
    PlayerManager* player_;

    Controls    mapping_;
    SDL_Event   event_;


public:

    ControlManager() {};
    ControlManager(LogManager* log, WindowManager* window, RoomManager* rooms, PlayerManager* player);

    void startUp(YAML::Node mapping);
    void shutDown();
    
    void handle_window(SDL_Event event);
    void handle_keyboard(SDL_Event event);
    mouse_click handle_mouse(SDL_Event event);
};
