#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"


class RenderManager;
class State;

class GameObject
{
protected:

    std::string id_;
    bool state_;
    bool debug_ { false };

public:

    GameObject() {}
    GameObject(std::string id, bool state = false);
    ~GameObject() {}

    std::string id();
    bool state();
    void state(bool new_state);
    
    virtual void update(RenderManager* renderer, int dt) = 0;
    virtual bool clicked(float x, float y) = 0;

    // Game Logic

    virtual void accept_click(State* handler, int x, int y, bool r) = 0;
    virtual void accept_over(State* handler, SDL_Event& event) = 0;
    virtual void accept_drag(State* handler, SDL_Event& event) = 0;

    // DEBUG

    void show_attributes();

};
 