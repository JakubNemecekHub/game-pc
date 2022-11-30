#include "../RenderManager.hpp"

#include <iostream>
#include <iostream>
#include <fstream>


SDL_Renderer* RenderManager::renderer_ = nullptr;


RenderManager::RenderManager(LogManager* log)
    : log_{log} {}


bool RenderManager::startUp(SDL_Window* window)
{
    log_->log("Starting Render Manager.");
    window_ = window;
    renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if ( renderer_ )
    {
        log_->log("Renderer created.");
        SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00);
        return true;
    }
    else
    {
        log_->error("SDL Error: ", SDL_GetError());
        return false;
    }
}


bool RenderManager::shutDown()
{
    SDL_DestroyRenderer(renderer_);
    log_->log("Renderer destroyed. Shutting down Render Manager.");
    return false;
}


/************************************************************************
    Load Stuff.
*************************************************************************/

/*
    Load and returns single SDL_Texture object from an image file.
    No src_rect.
*/
SDL_Texture* RenderManager::load_sdl_texture(std::string fileName)
{
    SDL_Texture* sdl_texture;
    sdl_texture = IMG_LoadTexture(renderer_, fileName.c_str());
    // Load texture.
    if ( sdl_texture == NULL )  // Given file not found.
    {
        log_->error("SDL_img Error: ", IMG_GetError());
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
        log_->error("SDL_img Error: ", IMG_GetError());
    }
    SDL_LockSurface(surface);
    return surface;
}


/*
    Convert SDL_Surface into SDL_Texture.
*/
SDL_Texture* RenderManager::texture_from_surface(SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer_, surface);
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
bool RenderManager::register_object(RenderableObject* r)
{
    if ( r->z_index() < MAX_LAYERS_ )
    {
        render_objects_.at(r->z_index()).push(r);
        return true;
    }
    return false;
    // else
    // {
    //     LogManager::GetInstance()->log_error("Z-index out of scope. May index is ", MAX_LAYERS_,
    //                                          ", was given ", r->z_index());
    // }
}


/*
    Render objects in order based on z-index of each object.
    z-index: 0 - room's background
             1 - room's ambient animations
             2..(N-1) - various stuff
             N - bitmaps
            (N+1) - polygons
    TO DO: The layering rules shouldn't be so strict. For example, we may want to show
    parts of the background above player. This way we create a feeling of "depth".
    Player object will move between various layers.
*/
void RenderManager::render()
{
    SDL_RenderClear(renderer_);
    RenderableObject* object {nullptr};
    for ( std::array<std::queue<RenderableObject*>, MAX_LAYERS_>::size_type i = 0; i < render_objects_.size(); i++ )
    {
        while ( !render_objects_[i].empty() )
        {
            object = render_objects_[i].front();
            render_objects_[i].pop();
            object->render(renderer_);
        }
    }
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer_);
}


/************************************************************************
    Helper functions.
*************************************************************************/

/*
    Sets scale of the given texture so that is fills the whole screen height.
    Used for loading room textures.
*/
void RenderManager::scale_full_h(Texture* texture)
{
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    // Scale.
    float scale = static_cast<float>(h) / texture->h();  // Use static_cast to really get a float.
    texture->scale(scale);
}
// void RenderManager::scale_full_h(const std::unique_ptr<Texture>& texture)
// {
//     // Get screen dimensions.
//     int w, h;
//     SDL_GetRendererOutputSize(renderer_, &w, &h);
//     // Scale.
//     float scale = static_cast<float>(h) / texture->h();  // Use static_cast to really get a float.
//     texture->scale(scale);
// }

// Returns current screen width.
int RenderManager::get_screen_width()
{
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    return w;
}