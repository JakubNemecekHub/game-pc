#include "../RenderableObject.h"

int RenderableObject::z_index()
{
    return z_index_;
}

void RenderableObject::set_z_index(int _z_index)
{
    // Add check that _z_index > 0? Do I want this restriction?
    z_index_ = _z_index;
}