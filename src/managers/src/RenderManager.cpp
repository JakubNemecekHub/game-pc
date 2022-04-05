#include "../RenderManager.hpp"
#include "../WindowManager.hpp"

#include <iostream>
#include <string>
#include <stack>            // Stack to store Textures to render
#include <unordered_map>    // Map of animations for a sprite
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../../components/Texture.hpp"


RenderManager* RenderManager::singleton_ = nullptr;
SDL_Renderer* RenderManager::renderer = nullptr;

RenderManager* RenderManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new RenderManager();
    }
    return singleton_;
}

void RenderManager::startUp()
{
    window = WindowManager::GetInstance()->window();
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if ( renderer )
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    }
}

void RenderManager::shutDown()
{
}


/************************************************************************
    Load Stuff.
*************************************************************************/
Texture* RenderManager::load_texture(std::string fileName)
{
    /*
        Loads and returns single Texture object from an image file.
        By default sets the Texture dimensions the same as the file dimensions are.
    */
    SDL_Texture* sdl_texture;
    SDL_Rect src_rect;
    sdl_texture = IMG_LoadTexture(renderer, fileName.c_str());
    // Load texture.
    if ( sdl_texture == NULL )  // Given file not found.
    {
        std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
    }
    else    // Texture loaded, get its dimension.
    {
        src_rect.x = 0;
        src_rect.y = 0;
        SDL_QueryTexture(sdl_texture, NULL, NULL, &src_rect.w, &src_rect.h);
    }
    Texture* texture = new Texture(sdl_texture, src_rect);
    return texture;
}

SDL_Texture* RenderManager::load_sdl_texture(std::string fileName)
{
    /*
        Loads and returns single SDL_Texture object from an image file.
        No src_rect.
    */
    SDL_Texture* sdl_texture;
    sdl_texture = IMG_LoadTexture(renderer, fileName.c_str());
    // Load texture.
    if ( sdl_texture == NULL )  // Given file not found.
    {
        std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
    }
    return sdl_texture;
}


/************************************************************************
    Render stuff.
*************************************************************************/
/*
    Should be able to register various types of Textures, because I need to be able to handle render order.
    e.g.
        room background
        room animation
        items
        player
*/
void RenderManager::register_room_texture(Texture* texture)
{
    room_background.push(texture);
}

void RenderManager::register_ambient_texture(Texture* texture)
{
    room_ambient.push(texture);
}

void RenderManager::render()
{
    /*
        Render order (Work in progress)
        1) Room background
        2) Room ambient animations
    */
    SDL_RenderClear(renderer);
    Texture* texture;
    // ad 1)
    while ( !room_background.empty() )
    {
        texture = room_background.top();
        room_background.pop();
        SDL_RenderCopyEx(renderer, texture->texture, &texture->src_rect, &texture->dest_rect, NULL, NULL, SDL_FLIP_NONE);
    }
    // ad 2)
    while ( !room_ambient.empty() )
    {
        texture = room_ambient.top();
        room_ambient.pop();
        SDL_RenderCopyEx(renderer, texture->texture, &texture->src_rect, &texture->dest_rect, NULL, NULL, SDL_FLIP_NONE);
    }
    SDL_RenderPresent(renderer);
}


/************************************************************************
    Helper functions.
*************************************************************************/
void RenderManager::scale_full_h(Texture* &texture)
{
    /*
        Sets scale of the given texture so that is fills the whole screen height.
        Used for loading room textures.
    */
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    // Scale.
    float scale = static_cast<float>(h) / texture->src_rect.h;  // Use static_cast to really get a float.
    texture->set_scale(scale);
}


int RenderManager::get_screen_width()
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return w;
}