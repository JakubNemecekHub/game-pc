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
    static_texture = nullptr;
    // polygon = nullptr;
    polygon = new Polygon();
}


void RenderManager::shutDown()
{
}


/************************************************************************
    Load Stuff.
*************************************************************************/

/*
    Load and return single Texture object from an image file.
    By default set the Texture dimensions the same as the file dimensions are.
*/
Texture* RenderManager::load_texture(std::string fileName)
{
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


/*
    Load and returns single SDL_Texture object from an image file.
    No src_rect.
*/
SDL_Texture* RenderManager::load_sdl_texture(std::string fileName)
{
    SDL_Texture* sdl_texture;
    sdl_texture = IMG_LoadTexture(renderer, fileName.c_str());
    // Load texture.
    if ( sdl_texture == NULL )  // Given file not found.
    {
        std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
    }
    return sdl_texture;
}

/*
    Load a bitmap file into a SDL_Surface. Lock the surface,
    so that it's pixels can be accessed.
*/
SDL_Surface* RenderManager::load_bitmap(std::string file_name)
{
    SDL_Surface* surface;
    surface = SDL_LoadBMP(file_name.c_str());
    if ( surface == NULL )  // Given file not found.
    {
        std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
    }
    SDL_LockSurface(surface);
    return surface;
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

// Register given backgroud texture of a room to be rendered.
void RenderManager::register_room_texture(Texture* texture)
{
    // room_background.push(texture);
    room_background = texture;
}


// Register given texture of ambient animation to be rendered.
void RenderManager::register_ambient_texture(Texture* texture)
{
    room_ambient.push(texture);
}


/*
    Register given bitmap surface of a hot-spot map to be rendered.
    For testing purposes only.
*/
void RenderManager::register_static_surface(SDL_Surface* surface)
{
    /*
        1) Create SDL_Texture from SDL_Surface
        2) Create Texture
        3) Set Texture src_ and destrect to that of the room_background Texture
    */
    static_texture = new Texture;
    static_texture->texture = SDL_CreateTextureFromSurface(renderer, surface);
    static_texture->set_src(room_background->src_rect);
    static_texture->set_dest(room_background->dest_rect);
    static_texture->set_scale(room_background->scale);
}


void RenderManager::register_polygon(Polygon* _polygon, float _scale, int _dx, int _dy)
{
    // Make a deep copy of the given polygon
    // otherwise we would scale and move the original walkarea
    // Polygon* polygon = new Polygon();
    *polygon = Polygon(*_polygon);
    // polygon = Polygon(_polygon);
    polygon->scale(_scale);
    polygon->move(_dx, _dy);
    std::cout << "Polygon registered" << std::endl;
}


/*
    Delist the bitmap surface so that it will no longer be rendered.
    For testing purposes only.
*/
void RenderManager::delist_static_surface()
{
    delete static_texture;
    static_texture = nullptr;
}


void RenderManager::delist_polygon()
{
    // polygon = nullptr;
    polygon->clear();
    std::cout << "Polygon delisted" << std::endl;
}


/*
    Render order (Work in progress)
    1) Room background
    2) Room ambient animations
    3) Static textures over everything else
    4) and on top af that polygon
*/
void RenderManager::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, room_background->texture, &room_background->src_rect, &room_background->dest_rect, 0.0f, NULL, SDL_FLIP_NONE);
    // ad 2)
    Texture* texture;
    while ( !room_ambient.empty() )
    {
        texture = room_ambient.top();
        room_ambient.pop();
        SDL_RenderCopyEx(renderer, texture->texture, &texture->src_rect, &texture->dest_rect, 0.0f, NULL, SDL_FLIP_NONE);
    }
    // ad 3)
    if ( static_texture != nullptr )
    {
        SDL_RenderCopyEx(renderer, static_texture->texture, &static_texture->src_rect, &static_texture->dest_rect, 0.0f, NULL, SDL_FLIP_NONE);
    }

    // ad 4)
    if ( !polygon->empty() )
    {
        // std::cout << "Rendering polygon." << std::endl;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        unsigned int i, j;
        j = polygon->size() - 1;
        for ( i = 0; i < polygon->size(); i++ )
        {
            SDL_RenderDrawLine(renderer,
                            polygon->vertices[j].x, polygon->vertices[j].y,
                            polygon->vertices[i].x, polygon->vertices[i].y);
            j = i;
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer);
}


/************************************************************************
    Helper functions.
*************************************************************************/

/*
    Sets scale of the given texture so that is fills the whole screen height.
    Used for loading room textures.
*/
void RenderManager::scale_full_h(Texture* &texture)
{
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    // Scale.
    float scale = static_cast<float>(h) / texture->src_rect.h;  // Use static_cast to really get a float.
    texture->set_scale(scale);
}

// Returns current screen width.
int RenderManager::get_screen_width()
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return w;
}