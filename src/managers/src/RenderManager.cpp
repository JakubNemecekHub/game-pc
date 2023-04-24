#include "../RenderManager.hpp"

#include <iostream>
#include <iostream>
#include <fstream>

#include "../../components/Sprite.hpp"


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
    Register a Sprite to be rendered.
*/
bool RenderManager::submit(Sprite* sprite)
{
    int z_index { sprite->z_index() };
    if ( z_index >= MAX_LAYERS_ )
    {
        log_->error("Z-index ", z_index, " out of scope (max ", MAX_LAYERS_, ")");
        return false;
    }
    render_queues_.at(z_index).push(MyType::Object(sprite));
    return true; // TO DO: make this make sense!
}
/*
    Register a Polygon to be rendered.
*/
bool RenderManager::submit(SDL_Surface* surface, SDL_FRect* dest_rect)
{
    surface_queue_.push(std::make_tuple(surface, dest_rect));
    return true;
}
/*
    Register a Polygon to be rendered.
*/
bool RenderManager::submit(Polygon* polygon)
{
    render_queues_.at(MAX_LAYERS_ - 1).push(MyType::Object(polygon));
    return true;
}
/*
    Register a Vector2D to be rendered.
*/
bool RenderManager::submit(Vector2D* vector2d)
{
    render_queues_.at(3).push(MyType::Object(vector2d));
    return true;
}


/*
    Render objects in order based on z-index.
    z-index: 0        : Room's background           (base layer)
             1        : Room's ambient animations
             2..(N-3) : various stuff                               (for N = 5, as it is now, this layer is number 2)
             N-2      : GUI                                         (for N = 5, as it is now, this layer is number 3)
             N-1      : polygons                    (topmost layer) (for N = 5, as it is now, this layer is number 4)
             Bitmaps are render over everything else.
*/
void RenderManager::render()
{
    SDL_RenderClear(renderer_);

    render_sprite();                                    // Render submitted renderable sprites objects 
    if ( !surface_queue_.empty() ) render_surface();    // Render bitmaps on top of everything.

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer_);
}

/************************************************************************
    Helper Render functions.
*************************************************************************/

/*
    Render all sprites in all render_queues_.
*/
void RenderManager::render_sprite()
{
    for ( std::array<std::queue<Sprite*>, MAX_LAYERS_>::size_type i = 0; i < render_queues_.size(); i++ )
    {
        while ( !render_queues_[i].empty() )
        {
            MyType::Object object { render_queues_[i].front() };
            object.render(renderer_);
            render_queues_[i].pop();
        }
    }
}


/*
    Render all bitmap surfaces from the surface_queue_.
*/
void RenderManager::render_surface()
{
    // Get window surface. We will blit all surfaces onto here.
    SDL_Surface* window_surface { SDL_GetWindowSurface(window_) };
    if ( window_surface == NULL )
    {
        log_->error("Cannot create window surface", SDL_GetError());
        return;
    }
    // Clear the window's surface to all black.
    SDL_LockSurface(window_surface);
    SDL_memset(window_surface->pixels, 0, window_surface->h * window_surface->pitch);
    SDL_UnlockSurface(window_surface);
    // Then copy all submitted surfaces onto the screen
    SDL_Surface* original_surface { nullptr };
    while ( !surface_queue_.empty() )
    {
        std::tuple<SDL_Surface*, SDL_FRect*> surface_info { surface_queue_.front() };
        original_surface = std::get<0>(surface_info);
        SDL_FRect* dest_rect = std::get<1>(surface_info);
        SDL_Rect final_dest_rect;
        final_dest_rect.x = (int)dest_rect->x;
        final_dest_rect.y = (int)dest_rect->y;
        final_dest_rect.w = (int)dest_rect->w;
        final_dest_rect.h = (int)dest_rect->h;
        SDL_Surface* converted_surface = SDL_ConvertSurface(original_surface, window_surface->format, 0);
        SDL_BlitScaled(converted_surface, NULL, window_surface, &final_dest_rect);
        SDL_FreeSurface(converted_surface);
        surface_queue_.pop();
    }
    // finally update the window surface with all the bitmaps blitted.
    SDL_UpdateWindowSurface(window_);
}


/************************************************************************
    Helper functions.
*************************************************************************/

/*
    Sets scale of the given texture so that is fills the whole screen height.
    Used for loading room textures.
*/
void RenderManager::scale_full_h(Sprite* sprite)
{
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    // Scale.
    float scale = static_cast<float>(h) / sprite->h();  // Use static_cast to really get a float.
    sprite->scale(scale);
}

void RenderManager::center_horizontally(Sprite* sprite)
{
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    float sprite_w { sprite->w() };
    float x = 0.5f * (w - sprite_w);
    sprite->x(x);
}

void RenderManager::center_vertically(Sprite* sprite)
{
    // Get screen dimensions.
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    float sprite_h { sprite->h() };
    float y = 0.5f * (h - sprite_h);
    sprite->y(y);
}

// Returns current screen width.
int RenderManager::get_screen_width()
{
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    return w;
}