#include "Game.hpp"

#include <yaml-cpp/yaml.h>


Game::Game()
{
    YAML::Node ini = YAML::LoadFile("D:/Prog/game_project/game/ini.yaml");
    // TO DO: error opening file

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


void Game::handle_click_(mouse_click mouse_click_data)
{
    if ( !std::get<1>(mouse_click_data) ) return;   // Check if there was a click to handle;
    int x { std::get<1>(mouse_click_data) };
    int y { std::get<2>(mouse_click_data) };
    bool right_click  { std::get<3>(mouse_click_data) };
    // Clicked on Item?
    Item* item { m_RoomManager.get_item(x, y) };
    if ( item )
    {
        // Yes, clicked on Item.
        if ( right_click )  // Look.
        {
            std::string observation { item->get_observation() };
            m_TextManager.register_text(observation, item->sprite()->x(), item->sprite()->y(),
                                        GREEN);
        }
        else                // Try to pick up.
        {
            if ( m_PlayerManager.inventory.has_space() )
            {
                m_PlayerManager.inventory.add(item);
                m_RoomManager.remove_item(item->id());
                m_TextManager.register_text(item->get_pick_observation(), item->sprite()->x(), item->sprite()->y(),
                                            PURPLE);
            }
            else
            {
                m_TextManager.register_text("My Inventory is full.", item->sprite()->x(), item->sprite()->x(),
                                            BEIGE);
            }
        }
    }
    else
    {
        // No, did't click on Item.
        // Clicked on Door?
        Door* door { m_RoomManager.get_door(x, y) };
        if ( door )
        {
            if ( right_click )
            {
                // Look
                std::string observation { door->get_observation() };
                m_TextManager.register_text(observation, x, y, GREEN);
            }
            else
            {
                if ( door->locked() )
                {
                    std::string key_id { door->key_id() };
                    if ( m_PlayerManager.inventory.has_item(key_id) )
                    {
                        door->unlock();
                        m_PlayerManager.inventory.remove(key_id);
                        m_TextManager.register_text("Unlocked.", x, y, PURPLE);
                    }
                    else
                    {
                        std::string observation { door->get_locked_observation() };
                        m_TextManager.register_text(observation, x, y, GREEN);
                    }
                }
                else
                {
                    m_TextManager.clean();
                    m_RoomManager.activate_room(door->target());
                }
            }
        }
        else
        {
            // No, didn't clicked door either.
            // Clicked hot spot?
            HotSpot* hot_spot { m_RoomManager.get_hot_spot(x, y) };
            if ( hot_spot )
            {
                std::string observation;
                if ( right_click )
                {
                    observation = hot_spot->get_observation();
                    m_TextManager.register_text(observation, x, y, BEIGE);
                }
                else
                {
                    observation = hot_spot->get_use_observation();
                    m_TextManager.register_text(observation, x, y, COLOR::GREEN);
                }
            }
        }
    }
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
        mouse_click mouse_click_data { m_ControlManager.handle_mouse(event_) };
        handle_click_(mouse_click_data);
    }
    // 2) Update.
    m_RoomManager.update(&m_RenderManager, dt);
    m_PlayerManager.update(&m_RenderManager, dt);
    m_TextManager.update(dt);
    // 3) Render.
    m_RenderManager.render();
}