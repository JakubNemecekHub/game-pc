#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

#include "../managers/RenderManager.hpp"
// #include "../components/Texture.hpp"
#include "../components/Animation.hpp"


class Sprite
{
public:
    // Fields
    std::unordered_map<std::string, Animation> animations;
    std::shared_ptr<Animation> current_animation; // unique_ptr?
    SDL_Texture* texture;   // Is this really needed? At the end the Texture from animation is shown.
    SDL_Rect src_rect;  // current render source <- must update to animate
    SDL_Rect dest_rect; // final render destination
    // int timer {0};
    // Methods
    void set_animation(std::string animation_name);
    void update(int dt);

};

#endif // SPRITE_COMPONENT_H