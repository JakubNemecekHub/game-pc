#include "../TextManager.hpp"

#include <iostream>


TextManager::TextManager(LogManager* log, RenderManager* renderer, YAML::Node ini)
    : log_{log}, renderer_{renderer}, text_timer_{0}
{
    font_file_ = ini["font"].as<std::string>();
    font_size_ = ini["size"].as<int>();
    max_duration_ = ini["max_duration"].as<int>();
    for (int i = 0; i < ini["colors"].size(); i++ )
    {
        colors_.emplace(std::piecewise_construct,
                std::forward_as_tuple(i),
                std::forward_as_tuple(ini["colors"][i][0].as<Uint8>(), ini["colors"][i][1].as<Uint8>(), ini["colors"][i][2].as<Uint8>()));
    }
}


/*
    Initialize TTF and load font.
*/
bool TextManager::startUp()
{
    log_->log("Starting Text Manager.");

    // Initialize SDL_ttf library
    if ( TTF_Init() == -1 )
    {
        log_->error("SDL_ttf Error: ", TTF_GetError());
        return false;
    }
    else{
        // Load font
        font_ = TTF_OpenFont(font_file_.c_str(), font_size_);
        if ( font_ == NULL )
        {
            log_->error("SDL_ttf Error: ", TTF_GetError());
            return false;
        }
    }
    return true;
}


/*
    Close font and shut down TTF.
*/
bool TextManager::ShutDown()
{
    TTF_CloseFont(font_);
    TTF_Quit();
    log_->log("Shutting down Text Manager.");
    return true;
}


void TextManager::register_text(std::string text, int x, int y, COLOR color)
{
    if ( display_text_ )
    {
        display_text_.release();
    }
    // Screen width used in wrapping text and moving it along the x axis
    int screen_width { renderer_->get_screen_width() };
    Color my_color { colors_.at(color) };
    SDL_Color _color = SDL_Color{my_color.r, my_color.g, my_color.b};
    // Create surface from text
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font_, text.c_str(), _color, screen_width);
    if ( text_surface == NULL )
    {
        log_->error("SDL_ttf Error: ", TTF_GetError());
        return;
    }
    // Create SDL_Texture from surface
    SDL_Texture* text_texture = renderer_->texture_from_surface(text_surface);
    if ( text_texture == NULL )
    {
        log_->error("SDL Error: ", SDL_GetError());
        return;
    }
    // Create Texture with SDL_Texture
    display_text_ = std::make_unique<Texture>(text_texture, 1, 3);
    display_text_->match_src_dimension();
    /*
        Set position
        Move text above mouse cursor.
        If end of the text texture would be rendered outside of the screen,
        move it along the x axis.
        If the width of the whole text is greater then the screen width than
        we are...
    */
    int final_x, final_y;
    final_x = screen_width - display_text_->src_rect().w;
    if ( final_x > x )
    {
        final_x = x;
    }
    final_y = y - display_text_->src_rect().h;
    display_text_->set_position(final_x, final_y);
    renderer_->register_object(display_text_.get());
    text_timer_ = 0;
}


void TextManager::update(int dt)
{
    if ( display_text_ )
    {
        if ( text_timer_ <= max_duration_ )
        {
            renderer_->register_object(display_text_.get());
            text_timer_ += dt;
        }
        else
        {
            display_text_ = nullptr;
            text_timer_ = 0;
        }
    }
}


void TextManager::clean() { display_text_.release(); }