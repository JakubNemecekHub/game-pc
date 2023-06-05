#pragma once

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"

class Managers;
class Sprite;
class Item;
class Door;
class HotSpot;
class Ambient;


class State
{
public:

    // State lifecycle

    virtual bool enter(Managers* managers) = 0;
    virtual bool exit() = 0;
    ~State() {};

    // Game loop methods

    // virtual void input(SDL_Event& event) = 0; 
    virtual void input_keyboard_(SDL_Event& event) = 0;
    virtual void input_mouse_(SDL_Event& event) = 0;
    void input(SDL_Event& event)
    {
        switch (event.type)
        {
        // case SDL_KEYDOWN:
        // case SDL_TEXTEDITING:
        // case SDL_TEXTINPUT:
        case SDL_KEYUP:
            input_keyboard_(event);
            break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            input_mouse_(event);
            break;
        }
    }
    virtual void update(int dt) = 0; 
    virtual void render() = 0; 
    
};


namespace Gameplay
{

class GameplayState : public State
{
public:
    virtual void visit_click(Item* item, SDL_Event& event) {}
    virtual void visit_click(Door* door, SDL_Event& event) {}
    virtual void visit_click(HotSpot* hot_spot, SDL_Event& event) {}
    virtual void visit_click(Ambient* ambient, SDL_Event& event) {}
    virtual void visit_over(Item* item, SDL_Event& event) {}
    virtual void visit_over(Door* door, SDL_Event& event) {}
    virtual void visit_over(HotSpot* hot_spot, SDL_Event& event) {}
    virtual void visit_over(Ambient* ambient, SDL_Event& event) {}
    virtual void visit_drag(Item* item, SDL_Event& event) {}
    virtual void visit_drag(Door* door, SDL_Event& event) {}
    virtual void visit_drag(HotSpot* hot_spot, SDL_Event& event) {}
    virtual void visit_drag(Ambient* ambient, SDL_Event& event) {}
};

} // namespace