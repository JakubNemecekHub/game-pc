#pragma once

#include <tuple>


namespace Mouse
{

typedef std::tuple<bool, int, int, bool> click;
struct Transform
{
    int x;
    int y;
    bool right_click;
};

inline auto destructure(Mouse::click data)
{
    struct result { int x; int y; bool right_click; };
    return result{std::get<1>(data), std::get<2>(data), std::get<3>(data)};
}

}