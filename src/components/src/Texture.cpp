#include "../Texture.hpp"

#include <SDL2/SDL.h>


Texture::Texture(SDL_Texture* _texture, SDL_Rect _src_rect, float _scale, int _z_index)
    : RenderableObject(_z_index), texture{_texture}, src_rect{_src_rect}, scale{_scale}
{
    dest_rect.x = 0;
    dest_rect.y = 0;
    dest_rect.w = src_rect.w * _scale;
    dest_rect.h = src_rect.h * _scale;
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}


/*
    Set src_rect values.
*/
void Texture::set_src(int _x, int _y, int _w, int _h)
{
    src_rect.x = _x;
    src_rect.y = _y;
    src_rect.w = _w;
    src_rect.h = _h;
}


/*
    Copy the values from a given SDL_Rect source.
*/
void Texture::set_src(SDL_Rect& source)
{
    src_rect.x = source.x;
    src_rect.y = source.y;
    src_rect.w = source.w;
    src_rect.h = source.h;
}


/*
    Set src_dest values.
*/
void Texture::set_dest(int _x, int _y, int _w, int _h)
{
    dest_rect.x = _x;
    dest_rect.y = _y;
    dest_rect.w = _w;
    dest_rect.h = _h;
}


/*
    Copy the values from a given SDL_Rect destination.
*/
void Texture::set_dest(SDL_Rect& source)
{
    dest_rect.x = source.x;
    dest_rect.y = source.y;
    dest_rect.w = source.w;
    dest_rect.h = source.h;
}

// Set position.
void Texture::set_position(int _x, int _y)
{
    dest_rect.x = _x;
    dest_rect.y = _y;
}

/*
    Set scale to a given value and scale
    width and height accordingly.
*/
void Texture::set_scale(float _scale){
    scale = _scale;
    dest_rect.w = src_rect.w * _scale;
    dest_rect.h = src_rect.h * _scale;
}

/*
    Set the width of dest_rect to a given value
    and scale height accordingly.
*/
void Texture::set_dimension_width(int _w)
{
    float scale;
    scale = static_cast<float>(dest_rect.w) / _w;
    dest_rect.w *= scale;
    dest_rect.h *= scale;
}

/*
    Set the height of dest_rect to a given value
    and scale width accordingly.
*/
void Texture::set_dimension_height(int _h)
{
    float scale;
    scale = static_cast<float>(dest_rect.h) / _h;
    dest_rect.w *= scale;
    dest_rect.h *= scale;
}


/*
    Set the width and height of dest_rect
*/
void Texture::set_dimensions(int _w, int _h)
{
   dest_rect.w = _w;
   dest_rect.h = _h;
}


/*
    Set the width and height of dest_rect to have the same
    values as src_rect, scaled by scale.
*/
void Texture::match_src_dimension()
{
   dest_rect.w = src_rect.w * scale;
   dest_rect.h = src_rect.h * scale;
}

/*
    Render texture using given renderer.
*/
void Texture::render(SDL_Renderer* renderer)
{
    // std::cout << "Really? " << texture << std::endl;
    SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect, 0.0f, NULL, SDL_FLIP_NONE);
}