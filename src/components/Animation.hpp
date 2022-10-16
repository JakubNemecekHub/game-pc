#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>   // unique_ptr

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
public:
    // Fields
    std::unique_ptr<Texture> texture;
    int current_frame;                  //this is src_rect. dest_rect and scale will be provided by RoomManager.
    long last_updated;
    std::vector<Frame> frames;
    int offset_x;                       // How is this used?
    int offset_y;                       // How is this used?

    Animation() {}                          // No-args Constructor
    Animation(const Animation&& source) {}  // Copy Constructor
    Animation(const Animation& source) {}   // Move Constructor
    ~Animation() {}                         // Destructor

    // Static Class Methods

    // Methods

    void reset();
    void update(int dt);
    void set_position(int _x, int _y);
};
