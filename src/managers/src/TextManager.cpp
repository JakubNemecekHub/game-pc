#include "../TextManager.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../LogManager.hpp"
#include "../RenderManager.hpp"
#include "../components/Texture.hpp"


TextManager* TextManager::singleton_ = nullptr;
const int TextManager::total_duration = 1250;


TextManager* TextManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new TextManager();
    }
    return singleton_;
}

/*
    Initialize TTF and load font.
    Default values are for now "Secret of Monkey Island" font of size 14.
*/
void TextManager::startUp()
{
    LogManager::GetInstance()->log_message("Starting Text Manager.");
    text_timer = 0;
    // Initialize SDL_ttf library
    if ( TTF_Init() == -1 )
    {
        LogManager::GetInstance()->log_error("SDL_ttf Error: ", TTF_GetError());
        // return false;
    }
    else{
        // Load font
        font = TTF_OpenFont("D:/Prog/cpp/game_engine_architecture/res/fonts/Secret of Monkey Island.ttf", 24);  // Full path for debug purposes
        if ( font == NULL )
        {
            LogManager::GetInstance()->log_error("SDL_ttf Error: ", TTF_GetError());
            // return false;
        }
    }
    // return true;
}

/*
    Close font and shut down TTF.
*/
void TextManager::ShutDown()
{
    TTF_CloseFont(font);
    TTF_Quit();
    LogManager::GetInstance()->log_message("Shutting down Text Manager.");
}


void TextManager::register_text(std::string text, int x, int y, COLOR color)
{
    if ( display_text )
    {
        display_text.release();
    }
    // Screen width used in wrapping text and moving it along the x axis
    int sreen_w{RenderManager::GetInstance()->get_screen_width()};
    SDL_Color _color;
    switch ( color )
    {
        case BEIGE:
            _color = SDL_Color{255, 183, 89};
            break;
        case PURPLE:
            _color = SDL_Color{214, 29, 208};
            break;
        default:
            _color = SDL_Color{255, 255, 255};
    }
    // Create surface from text
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), _color, sreen_w);
    if ( text_surface == NULL )
    {
        LogManager::GetInstance()->log_error("SDL_ttf Error: ", TTF_GetError());
        return;
    }
    // Create SDL_Texture from surface
    SDL_Texture* text_texture = RenderManager::GetInstance()->texture_from_surface(text_surface);
    if ( text_texture == NULL )
    {
        LogManager::GetInstance()->log_error("SDL Error: ", SDL_GetError());
        return;
    }
    // Create Texture with SDL_Texture
    display_text = std::make_unique<Texture>(text_texture, 1, 3);
    display_text->match_src_dimension();
    /*
        Set position
        Move text above mouse cursor
        If end of the text texture would be rendered outside of the screen,
        move it along the x axis.
        If the width of the whole text is rater then the screen widthm that
        we are...
    */
    int final_x, final_y;
    final_x = sreen_w - display_text->src_rect.w;
    if ( final_x > x )
    {
        final_x = x;
    }
    final_y = y - display_text->src_rect.h;
    display_text->set_position(final_x, final_y);
    text_timer = 0;
}


void TextManager::update(int dt)
{
    if ( display_text )
    {
        if ( text_timer <= TextManager::total_duration )
        {
            RenderManager::GetInstance()->register_object(display_text.get());
            text_timer += dt;
        }
        else
        {
            display_text = nullptr;
            text_timer = 0;
        }
    }
}


void TextManager::clean()
{
    display_text.release();
}