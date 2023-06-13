#include "../MenuState.hpp"

#include "../../Game.hpp"   // Managers
#include "../../components/Sprite.hpp"
#include "../Gameplay.NormalState.hpp"
#include "../ExitState.hpp"


MenuState MenuState::self_;
MenuState::MenuState() {}
MenuState* MenuState::get() { return &self_; }


bool MenuState::enter(Managers* managers)
{
    managers_ = managers;

    button_new_game_ = Button { "New Game", managers_->assets.sprite("button_animated"), 250, 120, 10, &managers_->text };
    button_load_ = Button { "Load Game", managers_->assets.sprite("button_animated"), 250, 220, 10, &managers_->text };
    button_exit_ = Button { "Exit", managers_->assets.sprite("button_animated"), 250, 320, 10, &managers_->text };

    return true;
}


bool MenuState::exit()
{
    return true;
}


void MenuState::input_keyboard_(SDL_Event& event)
{
    if ( event.type != SDL_KEYUP ) return;
    
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->state.next(Gameplay::Normal::get());
    }
}


void MenuState::input_mouse_(SDL_Event& event)
{
    auto[x, y] = managers_->control.mouse_position(event);
    switch (event.type)
    {
    case SDL_MOUSEBUTTONUP:
        if ( button_new_game_.clicked(x, y) ) managers_->state.next(Gameplay::Normal::get());
        if ( button_load_.clicked(x, y) )
        {
            managers_->state.next(Gameplay::Normal::get());
            managers_->items.load(&managers_->serial);
        }
        if ( button_exit_.clicked(x, y) ) managers_->state.next(ExitState::get());
        break;
    default:
        break;
    }
}


void MenuState::update(int dt)
{
    button_new_game_.update(&(managers_->renderer), dt);
    button_load_.update(&(managers_->renderer), dt);
    button_exit_.update(&(managers_->renderer), dt);
}


void MenuState::render()
{
    managers_->renderer.render();
}