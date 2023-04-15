#include "../ControlManager.hpp"

#include "../logic/States.hpp"


ControlManager::ControlManager(LogManager* log, StateManager* state, WindowManager* window)
    : log_{log}, state_{state}, window_{window} {}

void ControlManager::startUp(YAML::Node mapping)
{
    log_->log("Starting Control Manager.");
    mapping_ = Controls{
        SDL_GetKeyFromName(mapping["Inventory"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor_Items"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor_Hot_Spots"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor_Doors"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor_Walk_Polygon"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Editor_Bitmap"].as<std::string>().c_str()),
    };
    log_->log("Control Manager started.");
}


void ControlManager::shutDown()
{
    log_->log("Shutting down Control Manager.");
}


void ControlManager::handle_window(SDL_Event& event)
{
         if ( event.type == SDL_QUIT || (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP) )
         {
            state_->next(ExitState::get());
         } 
    else if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_F11 ) window_->toggle_fullscreen();
}


Controls ControlManager::mapping() { return mapping_; }
