#pragma once

#include "../utils/Mouse.hpp"

namespace Gameplay { class GameplayState; }
// class Gameplay::GameplayState;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept_click(Gameplay::GameplayState* handler, Mouse::Transform mouse_transform) = 0;
    virtual void accept_over(Gameplay::GameplayState* handler, Mouse::Transform mouse_transform) = 0;
};
 