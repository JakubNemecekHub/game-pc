#pragma once


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
    float x;
    float y;
    float xrel;
    float yrel;
    BUTTON b;
};

}