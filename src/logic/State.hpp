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
    virtual void visit(Item* item, Mouse::click mouse_click_data) = 0;
    virtual void visit(Door* door, Mouse::click mouse_click_data) = 0;
    virtual void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) = 0;
};

} // namespace