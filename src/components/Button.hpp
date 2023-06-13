#pragma once

#include <memory>
#include <string>
#include <functional>

// TO DO: Find a way which doesn't require introducing the sol2 library here.
// Can we use std::function somehow?
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include <sol/sol.hpp>


#include "GameObject.hpp"
#include "../components/Sprite.hpp"
class Sprite;
class RenderManager;
class TextManager;


class Button : public GameObject
{
private:

    Sprite* sprite_;
    std::string label_;
    std::unique_ptr<Sprite> label_sprite_;

public:

    Button() {}
    Button(std::string label, Sprite* sprite, float x, float y, float scale, TextManager* text, sol::function action);

    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

    sol::function action;

    // Game logic

    void accept_click(State* handler, int x, int y, bool r) override;
    void accept_over(State* handler, SDL_Event& event) override;
    void accept_drag(State* handler, SDL_Event& event) override;

};

