#include "../Texture.hpp"

#include <SDL2/SDL.h>


// Texture::~Texture()
// {
// }

void Texture::set_position(int _x, int _y)
{
    dest_rect.x = _x;
    dest_rect.y = _y;
}

void Texture::set_scale(float _scale){
    scale = _scale;
    dest_rect.w = src_rect.w * _scale;
    dest_rect.h = src_rect.h * _scale;
}

void Texture::set_dimension_width(int _w)
{
    float scale;
    scale = static_cast<float>(dest_rect.w) / _w;
    dest_rect.w *= scale;
    dest_rect.h *= scale;
}

void Texture::set_dimension_height(int _h)
{
    float scale;
    scale = static_cast<float>(dest_rect.h) / _h;
    dest_rect.w *= scale;
    dest_rect.h *= scale;
}

void Texture::set_dimensions(int _w, int _h)
{
    /*
        Sets the width and height of dest_rect
    */
   dest_rect.w = _w;
   dest_rect.h = _h;
}

void Texture::match_src_dimension()
{
    /*
        Sets the width and height of dest_rect to have the same
        values as src_rect, scaled by scale
    */
   dest_rect.w = src_rect.w * scale;
   dest_rect.h = src_rect.h * scale;
}