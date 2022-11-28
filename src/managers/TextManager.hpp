#pragma once

#include <string>
#include <memory>   // unique_ptr
#include <unordered_map>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "LogManager.hpp"
#include "RenderManager.hpp"
#include "../components/Texture.hpp"


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

    RenderManager*                  renderer_;
    LogManager*                     log_;
    std::string                     font_file_;
    int                             font_size_;
    TTF_Font*                       font_;              // .ttf font to use to display text
    int                             max_duration_;
    std::unique_ptr<Texture>        display_text_;      // One Texture representing a text to be shown
    int                             text_timer_;
    std::unordered_map<int, Color>  colors_;

public:

    TextManager() {};
    TextManager(LogManager* log, RenderManager* renderer, YAML::Node ini);

    bool startUp();
    bool ShutDown();

    void update(int dt);

    void register_text(std::string text, int x, int y, COLOR color=BEIGE);
    void clean();
};