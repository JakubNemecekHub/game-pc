#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <unordered_map>
#include <vector>


class Frame
{
public:
    SDL_Rect src_rect;
    int duration;
};

class Animation
{
public:
    SDL_Texture* sprite_sheet;
    int current_frame {0};
    long last_updated {0};
    std::vector<Frame> frames;
    int offset_x {0};
    int offset_y {0};
    // SDL_Rect dest_rect {0, 0, 0, 0}; // Is this used?
    // int speed                // To be able to play animation with different speeds?
};

class Sprite
{
public:
    // Fields
    std::unordered_map<std::string, Animation> animations;
    std::shared_ptr<Animation> current_animation; // unique_ptr?
    SDL_Texture* texture;
    SDL_Rect src_rect;  // current render source <- must update to animate
    SDL_Rect dest_rect; // final render destination
    // int timer {0};
    // Methods
    void setAnimation(std::string animation_name)
    {
        current_animation = std::make_shared<Animation>(animations.at(animation_name));
        src_rect = current_animation->frames[0].src_rect;
        texture = current_animation->sprite_sheet;
    }

    void update(int dt){
        // Look at the current frame of current animation
        // how long is it being shown and how long is it supposed to be shown
        // add dt to the time the frame is being shown
        //  if new value is larger that this frame duration than move to the next frame with its shown time set to 0
        //      e.g set src_rect
        //  if new value is lesser than duration that do nothing
        current_animation->last_updated += dt;
        if ( current_animation->last_updated > current_animation->frames[current_animation->current_frame].duration )
        {
            current_animation->current_frame = (current_animation->current_frame + 1) % current_animation->frames.size();
            current_animation->last_updated = 0;
            src_rect = current_animation->frames[current_animation->current_frame].src_rect;
        }
    }
};



#endif // SPRITE_COMPONENT_H