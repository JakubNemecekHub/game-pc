#pragma once

#include <string>
#include <memory>   // unique_ptr
#include <unordered_map>
#include <list>
#include <tuple>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "LogManager.hpp"
#include "RenderManager.hpp"


typedef std::tuple<std::unique_ptr<Sprite>, int> text_tuple;

class Sprite;

enum COLOR { BEIGE, PURPLE, GREEN, RED };

struct Color
{
    Uint8 r, g, b;
    Color(Uint8 _r, Uint8 _g, Uint8 _b)
        : r{_r}, g{_g}, b{_b} {}
};


/*
    Text Manager should handle all available fonts and their sizes and colours.
    Can I resize TTF_Font?

    It should also keep track of all the text that is supposed to be shown on the screen.
    It will display a text when asked and the keep the text on the screen for some time, or
    until some specific action, then it will hide the text.

*/
class TextManager
{
private:

    RenderManager*          renderer_;
    LogManager*             log_;
    
    std::string             font_file_;
    int                     font_size_;
    TTF_Font*               font_;              // .ttf font to use to display text
    int                     max_duration_;
    text_tuple              text_player_;
    std::unordered_map<std::string, std::unique_ptr<Sprite>> text_label_;
    std::list<text_tuple>   text_free_;
    std::unordered_map<int, Color>  colors_;


public:
    SDL_Texture* create_texture_(std::string text, COLOR color);
    void transform(std::unique_ptr<Sprite>& sprite, float x, float y);

    TextManager() {};
    TextManager(LogManager* log, RenderManager* renderer, YAML::Node ini);

    bool startUp();
    bool ShutDown();

    void update(int dt);
    
    Sprite* create_sprite(std::string text, COLOR color=COLOR::BEIGE);

    void submit_player(std::string text, float x, float y, COLOR color=COLOR::BEIGE);
    void submit_label(std::string id, std::string text, float x, float y, COLOR color=COLOR::BEIGE);
    void submit_free(std::string text, float x, float y, COLOR color=COLOR::BEIGE);
    void clean();
    void clean_player();
    void clean_label();               // Clean all labels
    void clean_label(std::string id); // Clean specific label
    
};