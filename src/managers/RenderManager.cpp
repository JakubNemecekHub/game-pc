#include "RenderManager.hpp"
#include "WindowManager.hpp"

#include <iostream>
#include <string>
#include <stack>            // Stack to store Textures to render
#include <unordered_map>    // Map of animations for a sprite
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../components/Texture.hpp"


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
Texture* RenderManager::loadTexture(const char* fileName)
{
    /*
        Loads and returns single Texture object from an image file.
        By default ets the Texture dimensions the same as the file dimensions are.
    */
    SDL_Texture* sdl_texture;
    SDL_Rect src_rect;
    sdl_texture = IMG_LoadTexture(renderer, fileName);
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


std::unordered_map<std::string, Animation> RenderManager::loadSprite(const char* file_name)
{
    /*
        Loads complete spritesheet with animation from a .anim file into a Sprite object.
    */
    // Open file
    std::ifstream animation_file {file_name, std::ios::in};
    // Temporary variables
    std::string value {};   // The value loaded from the file , more clever name?
    std::unordered_map<std::string, Animation> animations {};
    std::string animation_name {};
    std::string sprite_sheet_name {};
    Animation animation;
    Frame frame;
    // Loading loop
    if ( animation_file.is_open() )
    {
        while ( !animation_file.eof() )
        {
            animation_file >> value;
            if ( value == "ANIMATION" )
                animation_file >> animation_name;
            if ( value == "FILENAME" )
            {
                animation_file >> sprite_sheet_name;
                animation.sprite_sheet = IMG_LoadTexture(renderer, sprite_sheet_name.c_str());
                if ( animation.sprite_sheet == NULL )
                    std::cout << "SDL_img Error: " << IMG_GetError() << std::endl;
            }
            if ( value == "OFFSET" )
            {
                animation_file >> animation.offset_x;
                animation_file >> animation.offset_y;
            }
            if ( value == "FRAME" )
            {
                animation_file >> frame.src_rect.x;
                animation_file >> frame.src_rect.y;
                animation_file >> frame.src_rect.w;
                animation_file >> frame.src_rect.h;
                animation_file >> frame.duration;
                animation.frames.push_back(frame);
            }
            if ( value == "END" )
            {
                animations.insert({animation_name, animation});
                // Clear temp frames
                animation.frames.clear();
            }

        }
        // Close file after everyhing is loaded
        animation_file.close();
    }
    else
    {
        // File not opened
        std::cout << "File " << file_name << " cannot be opened." << std::endl;
    }
    return animations;
}


/************************************************************************
    Render stuff.
*************************************************************************/
void RenderManager::registerTexture(Texture* texture)
{
    /*
        Should be able to register various types of Textures, because I need to be able to handle render order.
        e.g.
            room background
            room animation
            items
            player
    */
    texture_stack.push(texture);
}


void RenderManager::render()
{
    // SDL_RenderClear(renderer);
    Texture* texture;
    while ( !texture_stack.empty() )
    {
        std::cout << "Rendering..." << std::endl;
        // Get Texture to be rendered.
        texture = texture_stack.top();
        texture_stack.pop();
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
    int w, h;   // Here we will store screen dimensions.
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