#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include <SDL2/SDL.h>

#include "RenderableObject.h"


/*
    SDL_Texture, together with source rectangle and its scale.
    Used for static visual elements, e.g. room background.
*/
class Texture : public RenderableObject
{
public:
    // It shows too much stuff to the outside world
    SDL_Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    float scale;

    // Constructors
    Texture() {};
    // Default scale is 1, default z_index is 0.
    // For a given scale. -> Should rework so that dest_rect is correct
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect, float _scale = 1, int _z_index = 0)
        : RenderableObject(_z_index), texture{_texture}, src_rect{_src_rect}, scale{_scale} {}
    // Destructor
    ~Texture();

    // Position, size and scale methods.

    void set_src(int _x, int _y, int _w, int _h);
    void set_src(SDL_Rect& source);
    void set_dest(int _x, int _y, int _w, int _h);
    void set_dest(SDL_Rect& source);
    void set_position(int _x, int _y);
    void set_scale(float _scale);
    void set_dimension_width(int _w);
    void set_dimension_height(int _h);
    void set_dimensions(int _w, int _h);
    void match_src_dimension();

    // Render methods.

    void render(SDL_Renderer* renderer);

};

#endif // TEXTURE_COMPONENT_H