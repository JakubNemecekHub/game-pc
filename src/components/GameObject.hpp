#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"


class RenderManager;
namespace Gameplay { class GameplayState; }

class GameObject
{
protected:

    std::string id_;
    bool state_;
    bool debug_ { false };

public:

    GameObject(std::string id, bool state = false);
    ~GameObject() {}

    std::string id();
    bool state();
    void state(bool new_state);
    
    virtual void update(RenderManager* renderer, int dt) = 0;
    virtual bool clicked(int x, int y) = 0;

    // Game Logic

    virtual void accept_click(Gameplay::GameplayState* handler, SDL_Event& event) = 0;
    virtual void accept_over(Gameplay::GameplayState* handler, SDL_Event& event) = 0;
    virtual void accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) = 0;

    // DEBUG

    void show_attributes();

};
 