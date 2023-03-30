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

#include "src/utils/Mouse.hpp"


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

    void input_(SDL_Event event);

public:

    Game();
    bool running();
    void update(int dt);

    // Game logic

    void visit(Item* item, Mouse::click mouse_click_data);
    void visit(Door* door, Mouse::click mouse_click_data);
    void visit(HotSpot* hot_spot, Mouse::click mouse_click_data);

    void input_keyboard(ScreenGameplayNormal* screen, SDL_Event event);
    void input_keyboard(ScreenGameplayInventory* screen, SDL_Event event);
    void input_mouse(ScreenGameplayNormal* screen, SDL_Event event);
    void input_mouse(ScreenGameplayInventory* screen, SDL_Event event);

};