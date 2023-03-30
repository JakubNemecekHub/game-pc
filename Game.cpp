#include "Game.hpp"

#include <yaml-cpp/yaml.h>

#define kkey event.key.keysym.sym // because i don't understand why this doesn'z work: SDL_KeyCode key = event.key.keysym.sym;


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
    m_RoomManager.startUp(&m_ItemManager, &m_AssetManager);
    m_ControlManager.startUp(ini["control"]);
    m_PlayerManager.startUp(&m_AssetManager);

    m_LogManager.log("All Managers started.");

}


bool Game::running() { return m_WindowManager.running; }


void Game::input_(SDL_Event event)
{
    // This will get called when there is a keyboard or mouse event.
    // Outcome of both keyboard and mouse events depends on the current active screen.
    ScreenControl* screen { m_ControlManager.screen() };
    if ( !screen )
    {
        m_LogManager.log("Something went wrong. I can't control anything");  
        return;
    }
    if ( event.type == SDL_KEYUP )
    {
        screen->accept_keyboard(this, event);
    }
    else if ( event.type == SDL_MOUSEBUTTONUP )
    {
        screen->accept_mouse(this, event);
    }
}


void Game::update(int dt)
{
    // 1) Handle input.
    while ( SDL_PollEvent(&event_) )
    {
        m_ControlManager.handle_window(event_);
        if ( event_.type == SDL_KEYUP || event_.type == SDL_MOUSEBUTTONUP )
        {
            input_(event_);
        }
    }
    // 2) Update.
    m_RoomManager.update(&m_RenderManager, dt);
    m_PlayerManager.update(&m_RenderManager, dt);
    m_TextManager.update(dt);
    // 3) Render.
    m_RenderManager.render();
}

/*****************************************************************************************************************
 * Game Logic
*****************************************************************************************************************/

/*
    Keyboard input
*/

/*
    Screen Gameplay Normal.
    Query active room for Game Object on the target location
*/
void Game::input_keyboard(ScreenGameplayNormal* screen, SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;

    const Controls mapping { m_ControlManager.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 

         if ( kkey == SDLK_ESCAPE ) m_WindowManager.close();
    // Fullscreen
    else if ( kkey == SDLK_F11 ) m_WindowManager.toggle_fullscreen();
    // Show click map
    else if ( kkey == mapping.KEY_HOT_SPOTS ) m_RoomManager.handle_keyboard(ACTION_ROOM::BITMAP);
    // Toggle walk area's polygon rendering
    else if ( kkey == mapping.KEY_WALK_POLYGON ) m_RoomManager.handle_keyboard(ACTION_ROOM::WALK_POLYGON);
    // Toggle items' click area's polygon rendering
    else if ( kkey == mapping.KEY_ITEM_POLYGON ) m_RoomManager.handle_keyboard(ACTION_ROOM::ITEM_POLYGON);
    // Toggle items' position vector rendering
    else if ( kkey == mapping.KEY_ITEM_VECTOR ) m_RoomManager.handle_keyboard(ACTION_ROOM::ITEM_VECTOR);
    // Show Inventory
    else if ( kkey == mapping.KEY_INVENTORY )
    {
        m_PlayerManager.inventory.show();
        m_ControlManager.screen(SCREEN::GAMEPLAY_INVENTORY);
    }
}


void Game::input_keyboard(ScreenGameplayInventory* screen, SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;

    const Controls mapping { m_ControlManager.mapping() };

    if ( kkey == mapping.KEY_INVENTORY )
    {
        m_PlayerManager.inventory.hide();
        m_ControlManager.screen(SCREEN::GAMEPLAY_NORMAL);
    }
    else
    {
        m_TextManager.register_text("But I won't do that.", 100, 100, COLOR::GREEN);
    }
}


/*
    Mouse input
*/

/*
    Screen Gameplay Normal.
    Query active Room for Game Object on the target location.  Resolve Game Oject interaction.
*/
void Game::input_mouse(ScreenGameplayNormal* screen, SDL_Event event)
{
    // We are assuming a mouse event did occur.
    Mouse::click mouse_data { m_ControlManager.handle_mouse(event) };   // This could be merged with the next line.
    auto [x, y, right_click] = Mouse::destructure(mouse_data);          // Get mouse click information.

    // auto [x, y, right_click] = m_ControlManager.mouse_data(event)    // Get mouse click information.


    GameObject* object { m_RoomManager.get_object(x, y) };              // Get object from Room Manager.
    if ( object) object->accept(this, mouse_data);                      // Do something with that object.
                                                                        // What do I need from the mouse data?
}


/*
    Screen Gameplay Inventory.
    Inventory is visible. Can interact only with the Inventory, no with the Room.
*/
void Game::input_mouse(ScreenGameplayInventory* screen, SDL_Event event)
{
    // We are assuming a mouse event did occur.
    Mouse::click mouse_data { m_ControlManager.handle_mouse(event) };   // This could be merged with the next line.
    auto [x, y, right_click] = Mouse::destructure(mouse_data);          // Get mouse click information.
    m_TextManager.register_text("Missed!", x, y, COLOR::PURPLE);
}


void Game::visit(Item* item, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look.
    {
        std::string observation { item->observation() };
        m_TextManager.register_text(observation, x, y, COLOR::GREEN);
    }
    else                // Try to pick up. TO DO: move to Inventory
    {
        if ( m_PlayerManager.inventory.has_space() )
        {
            m_PlayerManager.inventory.add(item);
            m_RoomManager.remove_item(item->id());
            m_TextManager.register_text(item->pick_observation(), x, y, COLOR::PURPLE);
        }
        else
        {
            m_TextManager.register_text("My Inventory is full.", x, y, COLOR::BEIGE);
        }
    }
}


void Game::visit(Door* door, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look
    {
        std::string observation { door->observation() };
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
                std::string observation { door->locked_observation() };
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


void Game::visit(HotSpot* hot_spot, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    std::string observation;
    if ( right_click )
    {
        observation = hot_spot->observation();
        m_TextManager.register_text(observation, x, y, COLOR::BEIGE);
    }
    else
    {
        observation = hot_spot->use_observation();
        m_TextManager.register_text(observation, x, y, COLOR::GREEN);
    }
}