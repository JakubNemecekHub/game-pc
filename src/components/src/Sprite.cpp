#include "../Sprite.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>


void Sprite::set_animation(std::string animation_name)
{
    current_animation = std::make_shared<Animation>(animations.at(animation_name));
    // src_rect = current_animation->frames[0].src_rect;
    current_animation->reset();
    // texture = current_animation->sprite_sheet;
}

void Sprite::update(int dt)
{
    current_animation->update(dt);
}