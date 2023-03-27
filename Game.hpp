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


class Game {
private:

    LogManager       m_LogManager;
    WindowManager    m_WindowManager;
    RenderManager    m_RenderManager;
    AssetManager     m_AssetManager;
    TextManager      m_TextManager;
    ItemManager      m_ItemManager;
    RoomManager      m_RoomManager;
    PlayerManager    m_PlayerManager;
    ControlManager   m_ControlManager;

    SDL_Event event_;

    void handle_click_(mouse_click mouse_click_data);

    // Game logic
    void clicked_item_(Item* item, mouse_click mouse_click_data);
    void clicked_door_(Door* door, mouse_click mouse_click_data);
    void clicked_hot_spot_(HotSpot* hot_spot, mouse_click mouse_click_data);

public:

    Game();
    bool running();
    void update(int dt);

};