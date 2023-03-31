#include "../ControlManager.hpp"

#include "../logic/State.hpp"


ControlManager::ControlManager(LogManager* log, StateManager* state, WindowManager* window)
    : log_{log}, state_{state}, window_{window} {}

void ControlManager::startUp(YAML::Node mapping)
{
    log_->log("Starting Control Manager.");
    mapping_ = Controls{
        SDL_GetKeyFromName(mapping["Inventory"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Walk_Polygon"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Item_Polygon"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Item_Vector"].as<std::string>().c_str()),
        SDL_GetKeyFromName(mapping["Hots_pots"].as<std::string>().c_str()),
    };
    log_->log("Control Manager started.");
}


void ControlManager::shutDown()
{
    log_->log("Shutting down Control Manager.");
}


void ControlManager::handle_window(SDL_Event event)
{
         if ( event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
         {
            state_->next(ExitState::get());
         } 
    else if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_F11 ) window_->toggle_fullscreen();
}


Mouse::click ControlManager::handle_mouse(SDL_Event event)
{
    if ( event.type != SDL_MOUSEBUTTONUP ) return std::make_tuple(false, 0, 0, false);
    
    int x, y;
    SDL_GetMouseState(&x, &y);
    bool right_click;
    right_click = ( event.button.button == SDL_BUTTON_RIGHT );
    return std::make_tuple(true, x, y, right_click);
}


auto ControlManager::mouse_data(SDL_Event event)
{
    struct result { int x; int y; bool right_click; };
    int x_, y_;
    SDL_GetMouseState(&x_, &y_);
    bool right_click { event.button.button == SDL_BUTTON_RIGHT };
    return result{x_, y_, right_click};
}


Controls ControlManager::mapping() { return mapping_; }
