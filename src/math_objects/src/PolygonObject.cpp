#include "../PolygonObject.hpp"

#include <iostream>

#include <SDL2/SDL.h>

#include "../math/Polygon.hpp"
#include "../components/RenderableObject.h"


void PolygonObject::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    unsigned int i, j;
    j = polygon->size() - 1;
    for ( i = 0; i < polygon->size(); i++ )
    {
        SDL_RenderDrawLine(renderer,
                           polygon->vertices[j].x, polygon->vertices[j].y,
                           polygon->vertices[i].x, polygon->vertices[i].y);
        j = i;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}