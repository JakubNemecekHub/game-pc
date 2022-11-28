#include "../Texture.hpp"

#include <iostream>


/*
    Create Texture object from SDL_Texture.
    Used to create Texture for text in TextManager and the Hot spot Texture in TextureManager.
*/
Texture::Texture(SDL_Texture* texture, float scale, int z_index)
    : RenderableObject(z_index), texture_{texture}, scale_{scale}
{
     // Set source rectangle
    src_rect_.x = 0;
    src_rect_.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src_rect_.w, &src_rect_.h);
    // Set destination rectangle, so that it doesn't contain garbage
    dest_rect_.x = 0;
    dest_rect_.y = 0;
    dest_rect_.w = 0;
    dest_rect_.h = 0;
}

/*
    Create a texture from a .png file.
    Used for all Textures in TextureManager.
*/
Texture::Texture(std::string file_name, RenderManager* renderer, float scale, int z_index)
    : RenderableObject(z_index), scale_{scale}
{
    // Load texture
    texture_ = renderer->load_sdl_texture(file_name);
    // Set source rectangle
    src_rect_.x = 0;
    src_rect_.y = 0;
    SDL_QueryTexture(texture_, NULL, NULL, &src_rect_.w, &src_rect_.h);
    // Set destination rectangle, so that it doesn't contain garbage
    dest_rect_.x = 0;
    dest_rect_.y = 0;
    dest_rect_.w = 0;
    dest_rect_.h = 0;
}


Texture::~Texture() { SDL_DestroyTexture(texture_); }


void Texture::set_texture(SDL_Texture* texture) { texture_ = texture; }


/*
    Set src_rect values.
*/
void Texture::set_src(int _x, int _y, int _w, int _h)
{
    src_rect_.x = _x;
    src_rect_.y = _y;
    src_rect_.w = _w;
    src_rect_.h = _h;
}


/*
    Copy the values from a given SDL_Rect source.
*/
void Texture::src_rect(SDL_Rect& source)
{
    src_rect_.x = source.x;
    src_rect_.y = source.y;
    src_rect_.w = source.w;
    src_rect_.h = source.h;
}


/*
    Get source rectangle.
*/
SDL_Rect Texture::src_rect() { return src_rect_; }


/*
    Set src_dest values.
*/
void Texture::set_dest(int _x, int _y, int _w, int _h)
{
    dest_rect_.x = _x;
    dest_rect_.y = _y;
    dest_rect_.w = _w;
    dest_rect_.h = _h;
}


/*
    Copy the values from a given SDL_Rect destination.
*/
void Texture::dest_rect(SDL_Rect& source)
{
    dest_rect_.x = source.x;
    dest_rect_.y = source.y;
    dest_rect_.w = source.w;
    dest_rect_.h = source.h;
}


/*
    Get source rectangle.
*/
SDL_Rect Texture::dest_rect() { return dest_rect_; }


// Set position.
void Texture::set_position(int x, int y)
{
    dest_rect_.x = x;
    dest_rect_.y = y;
}

/*
    Set scale to a given value and scale
    width and height accordingly.
*/
void Texture::scale(float scale)
{
    scale_ = scale;
    dest_rect_.w = static_cast<int>(src_rect_.w * scale);
    dest_rect_.h = static_cast<int>(src_rect_.h * scale);
}

/*
    Set the width of dest_rect to a given value
    and scale height accordingly.
*/
void Texture::set_dimension_width(int w)
{
    float scale;
    scale = static_cast<float>(dest_rect_.w) / w;
    dest_rect_.w = static_cast<int>(dest_rect_.w * scale);
    dest_rect_.h = static_cast<int>(dest_rect_.h * scale);
}

/*
    Set the height of dest_rect to a given value
    and scale width accordingly.
*/
void Texture::set_dimension_height(int h)
{
    float scale;
    scale = static_cast<float>(dest_rect_.h) / h;
    dest_rect_.w = static_cast<int>(dest_rect_.w * scale);
    dest_rect_.h = static_cast<int>(dest_rect_.h * scale);
}


/*
    Set the width and height of dest_rect
*/
void Texture::set_dimensions(int w, int h)
{
   dest_rect_.w = w;
   dest_rect_.h = h;
}


/*
    Set the width and height of dest_rect to have the same
    values as src_rect, scaled by scale.
*/
void Texture::match_src_dimension()
{
   dest_rect_.w = static_cast<int>(src_rect_.w * scale_);
   dest_rect_.h = static_cast<int>(src_rect_.h * scale_);
}

/*
    Get scale.
*/
float Texture::scale() { return scale_; }

/*
    Get x position.
*/
int Texture::x() { return dest_rect_.x; }

/*
    Get  position.
*/
int Texture::y() { return dest_rect_.y; }

/*
    Get  width.
*/
int Texture::w() { return dest_rect_.w; }

/*
    Get  height.
*/
int Texture::h() { return dest_rect_.h; }

/*
    Render texture using given renderer.
*/
void Texture::render(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, texture_, &src_rect_, &dest_rect_, 0.0f, NULL, SDL_FLIP_NONE);
}