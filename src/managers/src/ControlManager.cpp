#include "../ControlManager.hpp"

// #include "../logic/States.hpp"
#include "../logic/State.hpp"


ControlManager::ControlManager(LogManager* log, StateManager* state, WindowManager* window)
    : log_{log}, state_{state}, window_{window} {}


void ControlManager::shutDown()
{
    log_->log("Shutting down Control Manager.");
}


void ControlManager::handle_window(SDL_Event& event)
{
         if ( event.type == SDL_QUIT || (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYUP) )
         {
            state_->next("exit");
         } 
    else if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_F11 ) window_->toggle_fullscreen();
}
