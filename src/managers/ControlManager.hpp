#pragma once

#include <iostream>
#include <unordered_map>

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
    std::unordered_map<SDL_Keycode, std::string> mapping_m_;

public:

    ControlManager() {};
    ControlManager(LogManager* log, StateManager* state_,  WindowManager* window);

    void startUp(YAML::Node mapping);
    void shutDown();

    Controls mapping();
    
    void handle_window(SDL_Event& event);

    inline Mouse::Transform mouse_transform(SDL_Event& event)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return Mouse::Transform
        {
            static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel),
            event.button.button == SDL_BUTTON_RIGHT
        };
    }
    inline std::string key(SDL_Keycode key_code)
    {
        try
        {
            return mapping_m_.at(key_code);
        }
        catch(const std::out_of_range& e)
        {
            return "";
        }
        
    }
    
};
