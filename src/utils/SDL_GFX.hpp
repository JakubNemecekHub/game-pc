#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <cmath>
#include <algorithm>

namespace gfx
{

const SDL_Color DEFAULT_COLOR {255, 0, 147, SDL_ALPHA_OPAQUE};
 
 /*******************************************************************************
  * DRAW A LINE
  * TO DO:
  * > color selection
  * > Maybe use SDL_RenderDrawPoints after generating an array of points
 ********************************************************************************/
inline void plot_line_low(SDL_Renderer * renderer, int x0, int y0, int x1, int y1)
{
    // |m| <= 1, x is the fast direction
    int dx { x1 - x0 };
    int dy { std::abs(y1 - y0) };
    int yi { y1 < y0 ? -1 : 1 };
    int p { 2*dy - dx };
    int y { y0 };
    for (int x = x0; x <= x1; x++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        if ( p > 0 )
        {
            y += yi;
            p += 2*(dy - dx);
        }
        else
        {
            p = p + 2*dy;
        }
    }
}
 
inline void plot_line_high(SDL_Renderer * renderer, int x0, int y0, int x1, int y1)
{
    // |m| > 1, y is the fast direction
    int dx { std::abs(x1 - x0) };
    int dy { y1 - y0 };
    int xi { x1 < x0 ? -1 : 1 };
    int p { 2*dx - dy };
    int x { x0 };
 
    for (int y = y0; y <= y1; y++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        if ( p > 0 )
        {
            x += xi;
            p += 2*(dx - dy);
        }
        else
        {
            p += 2*dx;
        }
    }
}
 
inline void line(SDL_Renderer * renderer, int x0, int y0, int x1, int y1)
{
    if ( std::abs(y1 - y0) < std::abs(x1 - x0) )
    {
        if ( x0 > x1 ) plot_line_low(renderer, x1, y1, x0, y0);
        else           plot_line_low(renderer, x0, y0, x1, y1);
    }
    else
    {
        if ( y0 > y1 ) plot_line_high(renderer, x1, y1, x0, y0);
        else           plot_line_high(renderer, x0, y0, x1, y1);
 
    }
}


 /*******************************************************************************
  * DRAW A CIRCLE
 ********************************************************************************/
inline void circle(SDL_Renderer * renderer, int x0, int y0, int r)
{
    // Initial point is (0, r)
    int x   { r };
    int y   { 0 };
    int err { 1 - r }; // or { 3 - 2 * r }

    do
    {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        y++;
        if ( err > 0 )
        {
            x--;
            err += 2*y - 2*x + 1;  // or err += 2*(5 - 2*x + 2*y);
        }
        else
        {
            err += 2*y + 1;  // or err += 2*(3 + 2*y);
        }
    } while( x >= y );
}
 /*******************************************************************************
  * DRAW A DISC (A.K.A. FILLED CIRCLE)
 ********************************************************************************/
inline void disc(SDL_Renderer * renderer, int x0, int y0, int r)
{
    // Initial point is (0, r)
    int x   { r };
    int y   { 0 };
    int err { 1 - r }; // or { 3 - 2 * r }

    do
    {
        SDL_RenderDrawLine(renderer, x0 + x, y0 + y, x0 - x, y0 + y);
        SDL_RenderDrawLine(renderer, x0 + x, y0 - y, x0 - x, y0 - y);
        SDL_RenderDrawLine(renderer, x0 + y, y0 + x, x0 - y, y0 + x);
        SDL_RenderDrawLine(renderer, x0 + y, y0 - x, x0 - y, y0 - x);       
        y++;
        if ( err > 0 ) { x--; err += 2*y - 2*x + 1; }
        else { err += 2*y + 1; }
    } while( x >= y );
}




 /*******************************************************************************
  * DRAW A TRIANGLE
 ********************************************************************************/
inline void triangle(SDL_Renderer* renderer, int x0, int y0, int a)
{
    const float offset { a/2.0f };
    const std::vector<SDL_FPoint> vertices =
    {
        { static_cast<float>(x0 - offset), static_cast<float>(y0 + offset) },
        { static_cast<float>(x0 + offset), static_cast<float>(y0 + offset) },
        { static_cast<float>(x0), static_cast<float>(y0 - offset) },
        { static_cast<float>(x0 - offset), static_cast<float>(y0 + offset) },
    };
    SDL_RenderDrawLinesF(renderer, vertices.data(), 4);
}
inline void triangle_filled(SDL_Renderer* renderer, int x0, int y0, int a, SDL_Color color = DEFAULT_COLOR)
{
    const float offset { a/2.0f };
    const std::vector<SDL_Vertex> vertices =
    {
        { SDL_FPoint{ static_cast<float>(x0 - offset), static_cast<float>(y0 + offset) }, color, SDL_FPoint{ 0 }, },
        { SDL_FPoint{ static_cast<float>(x0 + offset), static_cast<float>(y0 + offset) }, color, SDL_FPoint{ 0 }, },
        { SDL_FPoint{ static_cast<float>(x0), static_cast<float>(y0 - offset) },          color, SDL_FPoint{ 0 }, },
    };
    SDL_RenderGeometry( renderer, nullptr, vertices.data(), 3, nullptr, 0 );
}


/*******************************************************************************
  * DRAW A SQUARE
 ********************************************************************************/
inline void square(SDL_Renderer* renderer, int x0, int y0, int a)
{
    const float offset { a/2.0f };
    const SDL_FRect dest_rect { x0 - offset, y0 - offset, static_cast<float>(a), static_cast<float>(a)};
    SDL_RenderDrawRectF(renderer, &dest_rect);
}
inline void square_filled(SDL_Renderer* renderer, int x0, int y0, int a)
{
    const float offset { a/2.0f };
    const SDL_FRect dest_rect { x0 - offset, y0 - offset, static_cast<float>(a), static_cast<float>(a)};
    SDL_RenderFillRectF(renderer, &dest_rect);
}

 /*******************************************************************************
  * DRAW A RHOMBUS
 ********************************************************************************/
inline void rhombus(SDL_Renderer* renderer, int x0, int y0, int a)
{
    const float offset { a/2.0f };
    const std::vector<SDL_FPoint> vertices =
    {
        { static_cast<float>(x0), static_cast<float>(y0 - offset) },
        { static_cast<float>(x0 + offset), static_cast<float>(y0) },
        { static_cast<float>(x0), static_cast<float>(y0 + offset) },
        { static_cast<float>(x0 - offset), static_cast<float>(y0) },
        { static_cast<float>(x0), static_cast<float>(y0 - offset) },
    };
    SDL_RenderDrawLinesF(renderer, vertices.data(), 5);
}
inline void rhombus_filled(SDL_Renderer* renderer, int x0, int y0, int a, SDL_Color color = DEFAULT_COLOR)
{
    const float offset { a/2.0f };
    // const SDL_Color color {255, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_FPoint vertex_a { static_cast<float>(x0), static_cast<float>(y0 - offset) };
    const SDL_FPoint vertex_b { static_cast<float>(x0 + offset), static_cast<float>(y0) };
    const SDL_FPoint vertex_c { static_cast<float>(x0), static_cast<float>(y0 + offset) };
    const SDL_FPoint vertex_d { static_cast<float>(x0 - offset), static_cast<float>(y0) };
    const SDL_FPoint c { 0 }; // tex_coord
    const std::vector<SDL_Vertex> vertices =
    {
        { vertex_a, color, c }, { vertex_b, color, c }, { vertex_c, color, c },
        { vertex_a, color, c }, { vertex_d, color, c }, { vertex_c, color, c },
    };
    SDL_RenderGeometry( renderer, nullptr, vertices.data(), 6, nullptr, 0 );
}

}   // namespace