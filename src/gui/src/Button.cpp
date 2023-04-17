#include "../Button.hpp"

#include <SDL2/SDL.h>

// #include "../../components/Sprite.hpp"
#include "../../managers/RenderManager.hpp"
#include "../../managers/TextManager.hpp"


Button::Button(std::string label, Sprite* sprite, int x, int y, float scale, TextManager* text)
    : label_{label}, sprite_{sprite}
{
    // Set up Sprite.
    sprite_->position(x, y);
    sprite_->scale(scale);
    // Set up a textual label inside the Sprite.
    label_sprite_ = std::make_unique<Sprite>(text->create_texture_(label_, COLOR::BEIGE), 1, 3);
    label_sprite_->depiction("text");
    label_sprite_->dimensions(sprite_->w(), sprite_->h());
    // label_sprite_->position(sprite_->x(), sprite_->y());
    int text_x { x + (sprite_->w() - label_sprite_->w())/2 };
    int text_y { y + (sprite_->h() - label_sprite_->h())/2 };
    label_sprite_->position(text_x, text_y);
}


bool Button::clicked(int x, int y)
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