#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "src/managers/LogManager.hpp"
#include "src/managers/RenderManager.hpp"
#include "src/managers/RoomManager.hpp"
#include "src/managers/TextManager.hpp"
#include "src/managers/WindowManager.hpp"
#include "src/managers/ItemManager.hpp"
#include "src/managers/AssetManager.hpp"
#include "src/managers/PlayerManager.hpp"
#include "src/managers/ControlManager.hpp"
#include "src/managers/StateManager.hpp"


class Managers
{
public:
    LogManager       log;
    WindowManager    window;
    RenderManager    renderer;
    AssetManager     assets;
    TextManager      text;
    ItemManager      items;
    RoomManager      rooms;
    PlayerManager    player;
    ControlManager   control;
    StateManager     state;

    Managers();
};


class Game {
private:

    Managers managers;

    SDL_Event event_;

public:

    Game();
    bool running();
    void update(int dt);

};