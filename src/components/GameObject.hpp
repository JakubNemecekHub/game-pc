#pragma once

#include "../utils/Mouse.hpp"


namespace Gameplay { class GameplayState; }

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept_click(Gameplay::GameplayState* handler, SDL_Event event) = 0;
    virtual void accept_over(Gameplay::GameplayState* handler, SDL_Event event) = 0;
    virtual void accept_drag(Gameplay::GameplayState* handler, SDL_Event event) = 0;
};
 