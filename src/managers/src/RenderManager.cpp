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
    render_queues_.at(z_index).push(sprite);
    return true; // TO DO: make this make sense!
}
/*
    Register a Polygon to be rendered.
*/
bool RenderManager::submit(SDL_Surface* surface, SDL_Rect* dest_rect)
{
    surface_queue_.push(std::make_tuple(surface, dest_rect));
    return true;
}
/*
    Register a Polygon to be rendered.
*/
bool RenderManager::submit(Polygon* polygon)
{
    math_queue_.push(MyType::Object(polygon));
    return true;
}
/*
    Register a Vector2D to be rendered.
*/
bool RenderManager::submit(Vector2D* vector2d)
{
    math_queue_.push(MyType::Object(vector2d));
    return true;
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

    render_sprite();                                    // Render sprites
    render_math();                                      // Render Polygons and Vectors on top of all textures.
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
    Sprite* sprite { nullptr };
    for ( std::array<std::queue<Sprite*>, MAX_LAYERS_>::size_type i = 0; i < render_queues_.size(); i++ )
    {
        while ( !render_queues_[i].empty() )
        {
            sprite = render_queues_[i].front();
            sprite->render(renderer_);
            render_queues_[i].pop();
        }
    }
}


/*
    Render all math objects in polygon_queue_ and vector_queue_
*/
void RenderManager::render_math()
{
    while ( !math_queue_.empty() )
    {
        MyType::Object object { math_queue_.front() };
        object.render(renderer_);
        math_queue_.pop();
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
    SDL_Rect* dest_rect;
    while ( !surface_queue_.empty() )
    {
        std::tuple<SDL_Surface*, SDL_Rect*> surface_info { surface_queue_.front() };
        original_surface = std::get<0>(surface_info);
        dest_rect = std::get<1>(surface_info);
        SDL_Surface* converted_surface = SDL_ConvertSurface(original_surface, window_surface->format, 0);
        SDL_BlitScaled(converted_surface, NULL, window_surface, dest_rect);
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
    int sprite_w { sprite->w() };
    int x = 0.5f * (w - sprite_w);
    sprite->x(x);
}

// Returns current screen width.
int RenderManager::get_screen_width()
{
    int w, h;
    SDL_GetRendererOutputSize(renderer_, &w, &h);
    return w;
}