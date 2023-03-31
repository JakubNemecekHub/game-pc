#pragma once

#include "../utils/Mouse.hpp"


class State;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept(State* handler, Mouse::click mouse) = 0;
};
 