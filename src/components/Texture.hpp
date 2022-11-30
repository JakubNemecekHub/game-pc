#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "RenderableObject.h"
#include "../managers/RenderManager.hpp"

class RenderManager;


/*
    SDL_Texture, together with source rectangle and its scale.
    Used for static visual elements, e.g. room background.
*/
class Texture : public RenderableObject
{
private:

    SDL_Texture*    texture_;
    SDL_Rect        src_rect_;
    SDL_Rect        dest_rect_;
    float           scale_;

public:

    // Constructors
    Texture() {};
    // Default scale is 1, default z_index is 0.
    Texture(SDL_Texture* texture, float scale = 1.0f, int _z_index = 0);
    Texture(std::string file_name, RenderManager* renderer, float _scale = 1.0f, int _z_index = 0);
    // Destructor
    ~Texture();

    void set_texture(SDL_Texture* texture);

    // Position, size and scale methods.

    void set_src(int _x, int _y, int _w, int _h);
    void src_rect(SDL_Rect& source);
    SDL_Rect src_rect();
    void set_dest(int _x, int _y, int _w, int _h);
    void dest_rect(SDL_Rect& source);
    SDL_Rect dest_rect();
    void set_position(int _x, int _y);
    void scale(float scale);
    float scale();
    void set_dimension_width(int _w);
    void set_dimension_height(int _h);
    void set_dimensions(int _w, int _h);
    void match_src_dimension();
    int x();
    int y();
    int w();
    int h();

    // Render methods.

    void render(SDL_Renderer* renderer);

};
