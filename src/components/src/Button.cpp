#include "../Button.hpp"

#include <SDL2/SDL.h>

#include "../managers/RenderManager.hpp"
#include "../managers/TextManager.hpp"
#include "../logic/State.hpp"


Button::Button(std::string label, Sprite* sprite, float x, float y, float scale, TextManager* text, sol::function action)
    : label_{label}, sprite_{sprite}, action{action}
{
    // Set up Sprite.
    sprite_->position(x, y);
    sprite_->scale(scale);
    sprite_->match_dimensions();
    // Set up a textual label inside the Sprite.
    label_sprite_ = std::make_unique<Sprite>(text->create_texture_(label_, COLOR::BEIGE), 1.0f, 3);
    label_sprite_->depiction("text");
    label_sprite_->match_dimensions();
    float text_x { x + (sprite_->w() - label_sprite_->w())/2 };
    float text_y { y + (sprite_->h() - label_sprite_->h())/2 };
    label_sprite_->position(text_x, text_y);
}


bool Button::clicked(float x, float y)
{
    bool x_in { (x > sprite_->x()) && (x <= sprite_->x() + sprite_->w())};
    bool y_in { (y > sprite_->y()) && (y <= sprite_->y() + sprite_->h())};
    return x_in && y_in;
}

void Button::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
    label_sprite_->update(renderer, dt);
}

void Button::accept_click(State* handler, int x, int y, bool r) { handler->visit_click(this, x, y, r); }
void Button::accept_over(State* handler, SDL_Event& event) { handler->visit_over(this, event); }
void Button::accept_drag(State* handler, SDL_Event& event) { handler->visit_drag(this, event); }