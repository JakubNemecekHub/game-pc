#include "../Sprite.hpp"


Sprite::Sprite(std::unordered_map<std::string, Animation*> animations)
    : animations_{animations} {}


void Sprite::set_animation(std::string id)
{
    current_animation_ = animations_.at(id);
    current_animation_->reset();
}