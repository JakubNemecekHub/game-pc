#include "Game.hpp"

#include <yaml-cpp/yaml.h>


Game::Game()
    // : visitor_{Visitor{this}}
{
    YAML::Node ini;
    try
    {
        ini = YAML::LoadFile("D:/Prog/game_project/game/ini.yaml");
    }
    catch(const YAML::BadFile& e)
    {
        std::cerr << "Cannot initialize game. " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
    
    // Start Log Manager as soon as possible
    m_LogManager = LogManager(ini["log"]);
    m_LogManager.startUp();


    // Start all other Managers
    m_WindowManager = WindowManager(&m_LogManager, ini["window"]);
    m_RenderManager = RenderManager(&m_LogManager);
    m_AssetManager = AssetManager(&m_LogManager);
    m_TextManager = TextManager(&m_LogManager, &m_RenderManager, ini["text"]);
    m_ItemManager = ItemManager(&m_LogManager);
    m_RoomManager = RoomManager(&m_LogManager);
    m_PlayerManager = PlayerManager(&m_LogManager);
    m_ControlManager = ControlManager(&m_LogManager, &m_WindowManager, &m_RoomManager, &m_PlayerManager);

    m_WindowManager.startUp();
    m_RenderManager.startUp(m_WindowManager.window());
    m_AssetManager.startUp(&m_RenderManager);
    m_TextManager.startUp();
    m_ItemManager.startUp(&m_AssetManager);
    m_RoomManager.startUp(&m_RenderManager, &m_ItemManager, &m_AssetManager);
    m_ControlManager.startUp(ini["control"]);
    m_PlayerManager.startUp(&m_AssetManager);

    m_LogManager.log("All Managers started.");

}


void Game::handle_click_(Mouse::click mouse_click_data)
{
    // Check if there was a click to handle. If not do nothing.
    if ( !std::get<1>(mouse_click_data) ) return;
    // Get mouse click information.
    auto [x, y, right_click] = Mouse::destructure(mouse_click_data);
    // Get object from Room Manager. 
    GameObject* object { m_RoomManager.get_object(x, y) };
    // Do something with that object.
    object->accept(&visitor_, &m_TextManager, &m_PlayerManager, &m_RoomManager, mouse_click_data);
}


bool Game::running() { return m_WindowManager.running; }


/*
    1) Handle input.
    2) Update.
    3) Render.
*/
void Game::update(int dt)
{
    // 1) Handle input.
    while ( SDL_PollEvent(&event_) )
    {
        m_ControlManager.handle_window(event_);
        m_ControlManager.handle_keyboard(event_);
        Mouse::click mouse_click_data { m_ControlManager.handle_mouse(event_) };
        handle_click_(mouse_click_data);
    }
    // 2) Update.
    m_RoomManager.update(&m_RenderManager, dt);
    m_PlayerManager.update(&m_RenderManager, dt);
    m_TextManager.update(dt);
    // 3) Render.
    m_RenderManager.render();
}