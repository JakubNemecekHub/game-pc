#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "../managers/RenderManager.hpp"
#include "../components/Texture.hpp"


class Frame
{
public:
    SDL_Rect src_rect;
    int duration;
    Frame() {};
    Frame(int _x, int _y, int _w, int _h, int _duration)
        : src_rect{_x, _y, _w, _h}, duration{_duration} {};
};


class Animation
{
private:

    Texture*            texture_;
    int                 current_frame_;  //this is src_rect. dest_rect and scale will be provided by RoomManager.
    long                last_updated_;
    std::vector<Frame>  frames_;
    // int offset_x;    // How is this used?
    // int offset_y;    // How is this used?

public:

    Animation() {}                          // No-args Constructor
    Animation(Texture* texture, std::vector<std::vector<int>> frames_meta);
    ~Animation() {}                         // Destructor

    // Methods

    // void position(int x, int y); // Done using Animation's texture
    void scale(float s);
    void reset();
    void update(RenderManager* renderer, int dt);

    Texture* texture();

};

