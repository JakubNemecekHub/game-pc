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


// struct Controls
// {
//     SDL_Keycode KEY_INVENTORY;
//     SDL_Keycode KEY_WALK_POLYGON;
//     SDL_Keycode KEY_ITEM_POLYGON;
//     SDL_Keycode KEY_ITEM_VECTOR;
//     SDL_Keycode KEY_HOT_SPOTS;
// };


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
    // Controls m_Controls;

    void handle_click_(mouse_click mouse_click_data);
    // void handle_keyboard_(SDL_Keycode key);

public:

    Game();
    bool running();
    void update(int dt);

};