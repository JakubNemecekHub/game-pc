#pragma once

#include <SDL2/SDL.h>

class RenderableObject
{
protected:
    int z_index_;   // Tells the renderer the order in which to render
                    // all Renderable objects.
                    // Protected, so that all children can access it.
public:

    // Constructor
    RenderableObject(int _z_index = 0)
        : z_index_{_z_index} {}
    // Virtual destructor
    virtual ~RenderableObject() {};

    // Methods

    virtual void render(SDL_Renderer* renderer) = 0;    // Virtual method
                                                        // Must be implemented by derived class.
    int z_index();
    void set_z_index(int _z_index);
};
