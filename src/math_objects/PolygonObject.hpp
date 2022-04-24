#ifndef POLYGON_OBJECT_H
#define POLYGON_OBJECT_H

#include <SDL2/SDL.h>

#include "../math/Polygon.hpp"
#include "../components/RenderableObject.h"


/*
    A visual representation of mathematical concept of Polygon.
*/
class PolygonObject : public RenderableObject
{
private:
    Polygon* polygon;
public:

    // Constructor
    PolygonObject(Polygon* _polygon, int _z_index=2)
        : RenderableObject(_z_index), polygon{_polygon} {}
    // Destructor
    ~PolygonObject() {};    // polygon is a pointer, but must not be destroyed

    // Methods

    void render(SDL_Renderer* renderer);
};

#endif // POLYGON_OBJECT_H