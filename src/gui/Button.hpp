#pragma once

#include <memory>
#include <string>


// class SDL_Texture;
#include "../components/Sprite.hpp"
class Sprite;
class RenderManager;
class TextManager;


class Button
{
private:

    Sprite* sprite_;
    std::string label_;
    std::unique_ptr<Sprite> label_sprite_;

public:

    Button() {}
    Button(std::string label, Sprite* sprite, int x, int y, float scale, TextManager* text);

    bool clicked(int x, int y);

    void update(RenderManager* renderer, int dt);

};

