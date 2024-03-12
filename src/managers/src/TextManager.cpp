#include "../TextManager.hpp"

#include <iostream>

#include "../../components/Sprite.hpp"


TextManager::TextManager(LogManager* log, RenderManager* renderer, YAML::Node ini)
    : log_{log}, renderer_{renderer}
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
    log_->log("Text Manager up and running.");
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


SDL_Texture* TextManager::create_texture_(std::string text, COLOR color)
{
    // Screen width used in wrapping text and moving it along the x axis
    int screen_width { renderer_->get_screen_width() };
    Color my_color { colors_.at(color) };
    SDL_Color _color = SDL_Color{my_color.r, my_color.g, my_color.b};
    // Create surface from text
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font_, text.c_str(), _color, screen_width);
    if ( text_surface == NULL )
    {
        log_->error("SDL_ttf Error: ", TTF_GetError());
        return nullptr;
    }
    // Create SDL_Texture from surface
    SDL_Texture* text_texture = renderer_->texture_from_surface(text_surface);
    // delete text_surface;
    SDL_FreeSurface(text_surface);
    if ( text_texture == NULL )
    {
        log_->error("SDL Error: ", SDL_GetError());
        return nullptr;
    }
    return text_texture;
}


// Transform the position and scale of a text texture relative to World space
void TextManager::transform(std::unique_ptr<Sprite>& sprite, Vector2D position)
{
    sprite->match_dimensions();
    /*
        Set position
        Move text above mouse cursor.
        If end of the text texture would be rendered outside of the screen,
        move it along the x axis.
        If the width of the whole text is greater then the screen width then
        we are...
    */
    int screen_width { renderer_->get_screen_width() };
    float final_x, final_y;
    final_x = screen_width - sprite->w();
    if ( final_x > position.x )
    {
        final_x = position.x;
    }
    final_y = position.y - sprite->h() / renderer_->camera_.zoom;
    sprite->position(Vector2D{final_x, final_y});
    sprite->scale(1 / renderer_->camera_.zoom);
    renderer_->submit(sprite.get());
}


// Transform the position and scale of a text texture relative to Screen space
void TextManager::transform_screen(std::unique_ptr<Sprite>& sprite, Vector2D position)
{
    sprite->match_dimensions();
    int screen_width { renderer_->get_screen_width() };
    float final_x, final_y;
    final_x = screen_width - (sprite->w() - renderer_->camera_.position.x) / renderer_->camera_.zoom;
    if ( final_x > position.x )
    {
        final_x = position.x;
    }
    // final_x = (x + renderer_->camera_.position.x - sprite->w());
    final_y = (position.y + renderer_->camera_.position.y - (sprite->h() * 0.9f));
    sprite->position(Vector2D{final_x, final_y});
    sprite->scale(1 / renderer_->camera_.zoom);
    renderer_->submit(sprite.get());
}


Sprite* TextManager::create_sprite(std::string text, COLOR color)
{
    SDL_Texture* text_texture = create_texture_(text, color);
    // return std::make_unique<Sprite>(text_texture, 1, 3);
    return new Sprite(text_texture, 1, 3);
}


void TextManager::submit_player(std::string text, Vector2D position, COLOR color)
{
    if ( std::get<0>(text_player_) )
    {
        std::get<0>(text_player_).release();
    }
    SDL_Texture* text_texture = create_texture_(text, color);
    std::get<0>(text_player_) = std::make_unique<Sprite>(text_texture, 1.0f, 3);
    std::get<0>(text_player_)->depiction("text");
    transform(std::get<0>(text_player_), position);
    std::get<1>(text_player_) = 0;
}


void TextManager::submit_label(std::string id, std::string text, Vector2D position, COLOR color)
{
    SDL_Texture* text_texture = create_texture_(text, color);
    if ( text_label_.count(id) == 1) clean_label(id);
    text_label_.emplace(std::piecewise_construct,
                        std::forward_as_tuple(id),
                        std::forward_as_tuple(std::make_unique<Sprite>(text_texture, 1.0f, 3)));
    text_label_.at(id)->depiction("text");
    transform_screen(text_label_.at(id), position);
}


void TextManager::submit_free(std::string text, Vector2D position, COLOR color)
{
    SDL_Texture* text_texture = create_texture_(text, color);                       // Create Sprite from text
    text_free_.emplace_back(std::make_unique<Sprite>(text_texture, 1.0f, 3), 0);    // Insert new Sprite into list
    std::unique_ptr<Sprite>& sprite { std::get<0>(text_free_.back()) };
    sprite->depiction("text");
    transform(sprite, position);                                                    // Set Sprite's dimensions
}


void TextManager::update(int dt)
{

    // Player's text
    if ( std::get<0>(text_player_) )
    {
        if ( std::get<1>(text_player_) <= max_duration_ )
        {
            renderer_->submit(std::get<0>(text_player_).get());
            std::get<1>(text_player_) += dt;
        }
        else
        {
            std::get<0>(text_player_).reset();
            std::get<1>(text_player_) = 0;
        }
    }

    // Label
    for (auto& label : text_label_ ) renderer_->submit(label.second.get());

    // Free text
    text_free_.remove_if([&](const text_tuple& text){ return std::get<1>(text) > max_duration_; }); // Remove expired text
    for ( auto& text : text_free_ )                                                                 // Update remaining text
    {
        renderer_->submit(std::get<0>(text).get());
        std::get<1>(text) += dt;
    }
}


void TextManager::clean()
{
    std::get<0>(text_player_).reset();
    std::get<1>(text_player_) = 0;
    // text_label_.reset();
    text_label_.clear();
    text_free_.clear();
}


void TextManager::clean_player()
{
    std::get<0>(text_player_).reset();
    std::get<1>(text_player_) = 0;
}


void TextManager::clean_label()
{
    text_label_.clear();
}


void TextManager::clean_label(std::string id)
{
    text_label_.erase(id);
}
