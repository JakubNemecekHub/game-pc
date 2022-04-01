#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H


class Texture
{
    // SDL_Texture, together with source rectangle 
public:
    SDL_Texture* texture;
    SDL_Rect src_rect;
    Texture(SDL_Texture* _texture, SDL_Rect _src_rect)
        : texture{_texture}, src_rect{_src_rect} {}
};

#endif // TEXTURE_COMPONENT_H