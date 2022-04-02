#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H


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
    // Default scale is 1.
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect)
        : texture{_texture}, src_rect{_src_rect}, dest_rect{_src_rect}, scale{1} {}
    // For a given scale. -> Should rework so that dest_rect is correct
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect, float _scale)
        : texture{_texture}, src_rect{_src_rect}, scale{_scale} {}
    // Methods
    void set_position(int _x, int _y)
    {
        dest_rect.x = _x;
        dest_rect.y = _y;
    }
    void set_scale(float _scale){
        scale = _scale;
        dest_rect.w = src_rect.w * _scale;
        dest_rect.h = src_rect.h * _scale;
    }
    void set_dimension_width(int _w)
    {
        float scale;
        scale = static_cast<float>(dest_rect.w) / _w;
        dest_rect.w *= scale;
        dest_rect.h *= scale;
    }
    void set_dimension_height(int _h)
    {
        float scale;
        scale = static_cast<float>(dest_rect.h) / _h;
        dest_rect.w *= scale;
        dest_rect.h *= scale;
    }
    // void set_dimensions(int _w, int _h)
    // {

    // }
};

#endif // TEXTURE_COMPONENT_H