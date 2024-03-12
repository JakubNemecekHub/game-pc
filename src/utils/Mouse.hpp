#pragma once

#include "../math/Vector2D.hpp"


namespace Mouse
{

enum class BUTTON
{
    NONE,
    LEFT,
    RIGHT,
};

struct Status
{
    Vector2D position;
    Vector2D relative;
    BUTTON b;
};

}
