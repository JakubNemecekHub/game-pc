#include "../IntroState.hpp"

#include "../../Game.hpp"   // Managers
#include "../components/Sprite.hpp"
// #include "../Gameplay.NormalState.hpp"
#include "../MenuState.hpp"


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


void IntroState::input_keyboard_(SDL_Event& event)
{
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->state.next(MenuState::get());
    }
}


void IntroState::input_mouse_(SDL_Event& event) {}


void IntroState::update(int dt)
{
    logo_->update(&(managers_->renderer), dt);
}


void IntroState::render()
{
    managers_->renderer.render();
}