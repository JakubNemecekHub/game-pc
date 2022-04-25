#include "../PolygonObject.hpp"

#include <iostream>

#include <SDL2/SDL.h>

#include "../math/Polygon.hpp"
#include "../components/RenderableObject.h"


/*
    Render scaled and moved version of a polygon.
*/
void PolygonObject::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    unsigned int i, j;
    float x1, y1, x2, y2;
    j = polygon.size() - 1;
    for ( i = 0; i < polygon.size(); i++ )
    {
        x1 = polygon.vertices[j].x * scale + dx;
        y1 = polygon.vertices[j].y * scale + dy;
        x2 = polygon.vertices[i].x * scale + dx;
        y2 = polygon.vertices[i].y * scale + dy;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        j = i;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}