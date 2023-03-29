#pragma once

#include "../utils/Mouse.hpp"


class Game;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept(Game* handler, Mouse::click mouse) = 0;
};
 