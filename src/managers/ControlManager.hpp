#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include <yaml-cpp/yaml.h>

#include "../utils/Mouse.hpp"
#include "LogManager.hpp"
#include "RoomManager.hpp"
#include "WindowManager.hpp"
#include "PlayerManager.hpp"


class Game;
class ScreenControl;

/*
    Input's result will depend on the current active screen.
    Possible Screens:
    - Menu
    - Gameplay normal
    - Gameplay inventory
*/
enum SCREEN
{
    MENU,
    GAMEPLAY_NORMAL,
    GAMEPLAY_INVENTORY
};


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

    ScreenControl* screen_;
    Controls       mapping_;
    SDL_Event      event_;


public:

    ControlManager() {};
    ControlManager(LogManager* log, WindowManager* window, RoomManager* rooms, PlayerManager* player);

    void startUp(YAML::Node mapping);
    void shutDown();

    Controls mapping();
    void screen(SCREEN type);
    ScreenControl* screen();
    
    void handle_window(SDL_Event event);
    Mouse::click handle_mouse(SDL_Event event);
    auto mouse_data(SDL_Event event);
    
};



class ScreenControl
{
public:
    ~ScreenControl() {}
    virtual void accept_keyboard(Game* handler, SDL_Event event) = 0;
    virtual void accept_mouse(Game* handler, SDL_Event event) = 0;
};

class ScreenGameplayNormal: public ScreenControl
{
public:
    void accept_keyboard(Game* handler, SDL_Event event) override;
    void accept_mouse(Game* handler, SDL_Event event) override;
};

class ScreenGameplayInventory : public ScreenControl
{
public:
    void accept_keyboard(Game* handler, SDL_Event event) override;
    void accept_mouse(Game* handler, SDL_Event event) override;
};
