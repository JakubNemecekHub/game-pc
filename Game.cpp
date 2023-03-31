#include "Game.hpp"

#include <yaml-cpp/yaml.h>

#include "src/logic/State.hpp"


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
    text = TextManager(&log, &renderer, ini["text"]);
    items = ItemManager(&log);
    rooms = RoomManager(&log);
    player = PlayerManager(&log);
    control = ControlManager(&log, &window);

    window.startUp();
    renderer.startUp(window.window());
    assets.startUp(&renderer);
    text.startUp();
    items.startUp(&assets);
    rooms.startUp(&items, &assets);
    control.startUp(ini["control"]);
    player.startUp(&assets);

    log.log("All Managers started.");
}
Game::Game()
{
    managers.log.log("Starting at default Intro State.");
    managers.state.start(IntroState::get(), &managers);
}

bool Game::running() { return managers.window.running; }

void Game::update(int dt)
{
    State* current_sate { managers.state.state() };
    // 1) Handle input
    while ( SDL_PollEvent(&event_) )
    {
        managers.control.handle_window(event_); // Global input
        if ( event_.type == SDL_KEYUP || event_.type == SDL_MOUSEBUTTONUP )
        {
            current_sate->input(event_);        // State specific input
        }
    }
    // 2) Update
    current_sate->update(dt);
    // 3) Change state
    managers.state.change(&managers);
    // 4) Render
    current_sate->render();
}
