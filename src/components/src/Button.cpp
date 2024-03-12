#include "../Button.hpp"

#include <SDL2/SDL.h>

#include "../logic/State.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/TextManager.hpp"
#include "../Sprite.hpp"


Button::Button(std::string label, Sprite* sprite, float x, float y, float scale, TextManager* text, sol::function action, Vertices vertices)
    : action{action}
{
    form_ = std::make_unique<ButtonForm>(sprite, vertices, text, label);
    form_->position(Vector2D{x, y});
    form_->scale(scale);
}


void Button::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}


void Button::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void Button::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }
