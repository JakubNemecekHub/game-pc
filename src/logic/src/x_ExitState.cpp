#include "../ExitState.hpp"

#include "../../Game.hpp"
#include "../components/Sprite.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


ExitState ExitState::self_;
ExitState::ExitState() {}
ExitState* ExitState::get() { return &self_; }


bool ExitState::enter(Managers* managers)
{
    managers_ = managers;
    outro_ = managers->assets.sprite("outro");
    outro_->scale_full_h();
    outro_->center();
    return true;
}


bool ExitState::exit()
{
    outro_ = nullptr;
    return true;
}


void ExitState::input_keyboard_(SDL_Event& event)
{
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->window.close();
    }
}


void ExitState::input_mouse_(SDL_Event& event)
{

}


void ExitState::update(int dt)
{
    outro_->update(&(managers_->renderer), dt);
}


void ExitState::render()
{
    managers_->renderer.render();
}