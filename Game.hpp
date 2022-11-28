#pragma once

#include <SDL2/SDL.h>

#include "src/managers/LogManager.hpp"
#include "src/managers/RenderManager.hpp"
#include "src/managers/RoomManager.hpp"
#include "src/managers/TextManager.hpp"
#include "src/managers/WindowManager.hpp"
#include "src/managers/ItemManager.hpp"
#include "src/managers/TextureManager.hpp"
#include "src/managers/PlayerManager.hpp"


class Game {
private:

    LogManager       m_LogManager;
    WindowManager    m_WindowManager;
    RenderManager    m_RenderManager;
    TextureManager   m_TextureManager;
    TextManager      m_TextManager;
    ItemManager      m_ItemManager;
    RoomManager      m_RoomManager;
    PlayerManager    m_PlayerManager;

    SDL_Event event_;

    void handle_click_(int x, int y, bool right_click);

public:

    Game();
    bool running();
    void update(int dt);

};