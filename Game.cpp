#include "Game.hpp"

#include <yaml-cpp/yaml.h>


#define kkey event.key.keysym.sym // because i don't understand why this doesn'z work: SDL_KeyCode key = event.key.keysym.sym;

Managers::Managers()
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
    log = LogManager(ini["log"]);
    log.startUp();

    // Start all other Managers
    window = WindowManager(&log, ini["window"]);
    renderer = RenderManager(&log);
    assets = AssetManager(&log);
    serial = SerializationManager(&log, &assets);
    text = TextManager(&log, &renderer, ini["text"]);
    items = ItemManager(&log, &assets, &serial);
    rooms = RoomManager(&log, &items, &assets);
    player = PlayerManager(&log, &assets, &items);
    control = ControlManager(&log, &state, &window);
    script = ScriptManager(&log);

    window.startUp();
    renderer.startUp(window.window());
    assets.startUp(&renderer);
    text.startUp();
    serial.startUp();
    control.startUp(ini["control"]);
    script.startUp(&state, &text, &player, &rooms, &window, &serial, &items, &assets);

    log.log("All Managers started.");
}


Game::Game()
{
    managers.log.log("Starting at default Intro State.");
    managers.state.start(&managers, "intro");
}


Game::~Game()
{
    managers.assets.shutDown();
}


bool Game::running() { return managers.window.running; }


void Game::update(int dt)
{
    State* current_sate { managers.state.state() };
    while ( SDL_PollEvent(&event_) )            // 1) Handle input
    {
        managers.control.handle_window(event_); // Global input
        current_sate->input(event_);            // State specific input
    }
    current_sate->update(dt);                   // 2) Update
    current_sate->render();                     // 3) Render
    managers.state.change();                    // 4) Change state
}
