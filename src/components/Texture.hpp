#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include <SDL2/SDL.h>


class Texture
{
    // SDL_Texture, together with source rectangle and its scale
    // Used for static visual elements, e.g. room background
public:
    SDL_Texture* texture;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    float scale;
    // Constructors
    Texture() {};
    // Default scale is 1.
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect)
        : texture{_texture}, src_rect{_src_rect}, dest_rect{_src_rect}, scale{1} {}
    // For a given scale. -> Should rework so that dest_rect is correct
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect, float _scale)
        : texture{_texture}, src_rect{_src_rect}, scale{_scale} {}
    // Destructor
    // ~Texture();

    // Methods

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
};

// class RoomTexture : public Texture
// {
// public:
//     SDL_Surface* click_map;
// }

#endif // TEXTURE_COMPONENT_H