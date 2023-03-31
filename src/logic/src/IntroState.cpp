#include "../IntroState.hpp"

#include "../../Game.hpp"
#include "../components/Sprite.hpp"
#include "../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


IntroState IntroState::self_;
IntroState::IntroState() {}
IntroState* IntroState::get() { return &self_; }

bool IntroState::enter(Managers* managers)
{
    managers_ = managers;
    logo_ = managers->assets.sprite("logo");
    logo_->scale_full_h();
    logo_->center();
    return true;
}

bool IntroState::exit()
{
    logo_ = nullptr;
    return true;
}

void IntroState::input(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;
    
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->state.next(Gameplay::Normal::get());
    }

}

void IntroState::update(int dt)
{
    logo_->update(&(managers_->renderer), dt);
}

void IntroState::render()
{
    managers_->renderer.render();
}