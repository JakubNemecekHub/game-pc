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

    button_new_game_ = Button { "New Game", managers_->assets.sprite("button_animated"), 10, 20, 10, &managers_->text };
    button_load_ = Button { "Load Game", managers_->assets.sprite("button_animated_1"), 10, 120, 10, &managers_->text };
    button_exit_ = Button { "Exit", managers_->assets.sprite("button_animated_2"), 10, 220, 10, &managers_->text };

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


void MenuState::input(SDL_Event& event)
{
    switch (event.type)
    {
    // case SDL_KEYDOWN:
    // case SDL_TEXTEDITING:
    // case SDL_TEXTINPUT:
    case SDL_KEYUP:
        input_keyboard_(event);
        break;
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
        input_mouse_(event);
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