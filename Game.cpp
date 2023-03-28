#include "Game.hpp"

#include <yaml-cpp/yaml.h>


Game::Game()
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

inline auto destructure(mouse_click data)
{
    struct result { int x; int y; bool right_click; };
    return result{std::get<1>(data), std::get<2>(data), std::get<3>(data)};
}

void Game::clicked_item_(Item* item, mouse_click mouse_click_data)
{
    auto [x, y, right_click] = destructure(mouse_click_data);
    if ( right_click )  // Look.
    {
        std::string observation { item->get_observation() };
        m_TextManager.register_text(observation, x, y, COLOR::GREEN);
    }
    else                // Try to pick up. TO DO: move to Inventory
    {
        if ( m_PlayerManager.inventory.has_space() )
        {
            m_PlayerManager.inventory.add(item);
            m_RoomManager.remove_item(item->id());
            m_TextManager.register_text(item->get_pick_observation(), x, y, COLOR::PURPLE);
        }
        else
        {
            m_TextManager.register_text("My Inventory is full.", x, y, COLOR::BEIGE);
        }
    }
}

void Game::clicked_door_(Door* door, mouse_click mouse_click_data)
{
    auto [x, y, right_click] = destructure(mouse_click_data);
    if ( right_click )  // Look
    {
        std::string observation { door->get_observation() };
        m_TextManager.register_text(observation, x, y, COLOR::GREEN);
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
                m_TextManager.register_text("Unlocked.", x, y, COLOR::PURPLE);
            }
            else
            {
                std::string observation { door->get_locked_observation() };
                m_TextManager.register_text(observation, x, y, COLOR::GREEN);
            }
        }
        else
        {
            m_TextManager.clean();
            m_RoomManager.activate_room(door->target());
        }
    }
}

void Game::clicked_hot_spot_(HotSpot* hot_spot, mouse_click mouse_click_data)
{
    auto [x, y, right_click] = destructure(mouse_click_data);
    std::string observation;
    if ( right_click )
    {
        observation = hot_spot->get_observation();
        m_TextManager.register_text(observation, x, y, COLOR::BEIGE);
    }
    else
    {
        observation = hot_spot->get_use_observation();
        m_TextManager.register_text(observation, x, y, COLOR::GREEN);
    }
}

void Game::handle_click_(mouse_click mouse_click_data)
{
    // Check if there was a click to handle. If not do nothing.
    if ( !std::get<1>(mouse_click_data) ) return;
    // Get mouse click information.
    auto [x, y, right_click] = destructure(mouse_click_data);
    // Get object from Room Manager. 
    Item*    item     { m_RoomManager.get_item(x, y)     };
    Door*    door     { m_RoomManager.get_door(x, y)     };
    HotSpot* hot_spot { m_RoomManager.get_hot_spot(x, y) };
    // Do something with that object.
         if ( item )     clicked_item_(item, mouse_click_data);
    else if ( door )     clicked_door_(door, mouse_click_data);
    else if ( hot_spot ) clicked_hot_spot_(hot_spot, mouse_click_data);
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