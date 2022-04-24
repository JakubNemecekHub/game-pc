#include "../RenderManager.hpp"

#include <iostream>
#include <string>
#include <array>
#include <queue>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../WindowManager.hpp"
#include "../../components/RenderableObject.h"
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

/*
    Convert SDL_Surface into SDL_Texture.
*/
SDL_Texture* RenderManager::texture_from_surface(SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer, surface);
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


/*
    Register a RenderableObject based on its z_index.
*/
void RenderManager::register_object(RenderableObject* r)
{
    if ( r->z_index() < MAX_LAYERS )
        render_objects.at(r->z_index()).push(r);
    else
        std::cout << "Can't handle that." << std::endl;
}


/*
    Render objects in order based on z-index of each object.
    z-index: 0 - room's background
             1 - room's ambient animations
             2..(N-1) - various stuff
             N - bitmaps
            (N+1) - polygons
    TO DO: The layering rules shouldn't be so strict. For exaplme, we may want to show
    parts of the backgroud above player. This way we create a feeling of "depth".
    Player object will move between various layers.
*/
void RenderManager::render()
{
    SDL_RenderClear(renderer);
    RenderableObject* object;
    for ( unsigned int i = 0; i < render_objects.size(); i++ )
    {
        while ( !render_objects[i].empty() )
        {
            object = render_objects[i].front();
            render_objects[i].pop();
            object->render(renderer);
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