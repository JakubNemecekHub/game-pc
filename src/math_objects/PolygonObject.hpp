#pragma once

#include <SDL2/SDL.h>

#include "../math/Polygon.hpp"
#include "../components/RenderableObject.h"


/*
    A visual representation of mathematical concept of Polygon.
    It is scaled end moved.
*/
class PolygonObject : public RenderableObject
{
private:
    Polygon& polygon;
    float scale;
    int dx, dy;
public:

    // Constructor
    PolygonObject(Polygon& _polygon, float _scale=1.0f, int _dx=0, int _dy=0, int _z_index=3)
        : RenderableObject(_z_index), polygon{_polygon}, scale{_scale}, dx{_dx}, dy{_dy} {}
    // Destructor
    ~PolygonObject() {};    // polygon is a reference, must not/cannot be destroyed

    // Methods

    void render(SDL_Renderer* renderer);
};
