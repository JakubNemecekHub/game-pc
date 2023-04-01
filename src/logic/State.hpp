#pragma once

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"

class Managers;
class Sprite;
class Item;
class Door;
class HotSpot;


class State
{
public:

    // State lifecycle

    virtual bool enter(Managers* managers) = 0;
    virtual bool exit() = 0;
    ~State() {};

    // Game loop methods

    virtual void input(SDL_Event event) = 0; 
    virtual void update(int dt) = 0; 
    virtual void render() = 0; 
    
};


namespace Gameplay
{

class GameplayState : public State
{
public:
    virtual void visit_click(Item* item, Mouse::Transform mouse_transform) = 0;
    virtual void visit_click(Door* door, Mouse::Transform mouse_transform) = 0;
    virtual void visit_click(HotSpot* hot_spot, Mouse::Transform mouse_transform) = 0;
    virtual void visit_over(Item* item, Mouse::Transform mouse_transform) = 0;
    virtual void visit_over(Door* door, Mouse::Transform mouse_transform) = 0;
    virtual void visit_over(HotSpot* hot_spot, Mouse::Transform mouse_transform) = 0;
};

} // namespace