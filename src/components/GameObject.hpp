#pragma once

#include "../utils/Mouse.hpp"

namespace Gameplay { class GameplayState; }
// class Gameplay::GameplayState;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept(Gameplay::GameplayState* handler, Mouse::click mouse) = 0;
};
 