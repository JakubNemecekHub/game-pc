#include "Game.hpp"

#include <yaml-cpp/yaml.h>


Game::Game()
{
    YAML::Node ini = YAML::LoadFile("D:/Prog/game_project/game/ini.yaml");

    // Start Log Manager as soon as possible
    m_LogManager = LogManager(ini["log"]);
    m_LogManager.startUp();

    // Load in key bindings
    YAML::Node bindings = ini["control"];
    m_Controls = Controls{bindings["Inventory"].as<int>(),
                          bindings["Polygon"].as<int>(),
                          bindings["Hots_pots"].as<int>()
                         };

    // Start all other Managers
    m_WindowManager = WindowManager(&m_LogManager, ini["window"]);
    m_RenderManager = RenderManager(&m_LogManager);
    m_TextureManager = TextureManager(&m_LogManager);
    m_TextManager = TextManager(&m_LogManager, &m_RenderManager, ini["text"]);
    m_ItemManager = ItemManager(&m_LogManager);
    m_RoomManager = RoomManager(&m_LogManager);
    m_PlayerManager = PlayerManager();

    m_WindowManager.startUp();
    m_RenderManager.startUp(m_WindowManager.window());
    m_TextureManager.startUp(&m_RenderManager);
    m_TextManager.startUp();
    m_ItemManager.startUp(&m_TextureManager);
    m_RoomManager.startUp(&m_RenderManager, &m_ItemManager, &m_TextureManager);

    m_LogManager.log("All Managers started.");
}

void Game::handle_click_(int x, int y, bool right_click)
{
    // Clicked on Item?
    Item* item { m_RoomManager.get_item(x, y) };
    if ( item )
    {
        // Yes, clicked on Item.
        if ( right_click )  // Look.
        {
            std::string observation { item->get_observation() };
            m_TextManager.register_text(observation, item->texture()->x(), item->texture()->y(),
                                        GREEN);
        }
        else                // Try to pick up.
        {
            if ( m_PlayerManager.inventory.has_space() )
            {
                m_PlayerManager.inventory.add(item);
                m_RoomManager.remove_item(item->id());
                m_TextManager.register_text(item->get_pick_observation(), item->texture()->x(), item->texture()->y(),
                                            PURPLE);
            }
            else
            {
                m_TextManager.register_text("My Inventory is full.", item->texture()->x(), item->texture()->x(),
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


void Game::handle_keyboard_(SDL_Keycode key)
{
    // Fullscreen
    if ( key == SDLK_F11 )
    {
        m_WindowManager.toggle_fullscreen();
    }
    // Toggle Static texture
    else if ( key == m_Controls.KEY_HOT_SPOTS )
    {
        m_RoomManager.handle_keyboard("bitmap");
    }
    // Toggle polygon rendering
    else if ( key == m_Controls.KEY_POLYGON )
    {
        m_RoomManager.handle_keyboard("polygon");
    }
    // Show Inventory
    else if ( key == m_Controls.KEY_INVENTORY )
    {
        m_PlayerManager.inventory.print(&m_LogManager);
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
        // Close Window
        if ( event_.type == SDL_QUIT || event_.key.keysym.sym == SDLK_ESCAPE)
            m_WindowManager.close();
        // Mouse click
        if ( event_.type == SDL_MOUSEBUTTONUP )
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            bool right_click { event_.button.button == SDL_BUTTON_RIGHT };
            handle_click_(x, y, right_click);
        }
        // Keyboard
        if ( event_.type == SDL_KEYUP )
        {
            handle_keyboard_(event_.key.keysym.sym);
        }
    }
    // 2) Update.
    m_RoomManager.update(&m_RenderManager, dt);
    m_TextManager.update(dt);
    // 3) Render.
    m_RenderManager.render();
}