#pragma once

#include <string>
#include <memory>   // unique_ptr

#include <SDL2/SDL_ttf.h>

#include "../components/Texture.hpp"


enum COLOR { BEIGE, PURPLE };


/*
    Text Manager should handle all aviable fonts and thei sizes and colours.
    Can I resize TTF_Font?

    It should also keep track of all the text that is supposed to be shown on the screen.
    It will display a text when asked amd the keep the texto on the screen for some time, or
    until some speficic action, then it will hide the text.

*/
class TextManager
{
private:
    static TextManager* singleton_;
    TextManager() {};
    TTF_Font* font;                         // .ttf font to use to display text
    static const int total_duration;
    std::unique_ptr<Texture> display_text;  // One Texture representing a text to be shown
                                            // TO DO: support multiple text on the screen
    int text_timer;
public:
    static TextManager* GetInstance();
    ~TextManager() {};
    void startUp();
    void ShutDown();
    void update(int dt);

    void register_text(std::string text, int x, int y, COLOR color=BEIGE);
    void clean();
};