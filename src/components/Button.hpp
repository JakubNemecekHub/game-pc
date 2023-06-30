#pragma once

#include <string>

#include "GameObject.hpp"
#include "../utils/Sol.hpp"

class RenderManager;
class TextManager;


class Button : public GameObject
{
public:

    sol::function action;

    Button() {}
    Button(std::string label, Sprite* sprite, float x, float y, float scale, TextManager* text, sol::function action, Vertices vertices);
    Button(const Button& source) {};

    void update(RenderManager* renderer, int dt) override;

    // Game logic

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

};

