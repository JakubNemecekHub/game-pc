#pragma once

#include "../math/Vector2D.hpp"


class Transform
{
public:
    Vector2D position;
    Vector2D destination;
    bool is_walking;
    int speed;
    int scale;
};
