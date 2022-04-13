#include "../polygon.hpp"

#include "../Vector2D.hpp"


Polygon::Polygon(std::vector<Vector2D> _vertices)
    : vertices{_vertices} {}


// Add vertex at the end of the list of verticies.
void Polygon::add_vertex(Vector2D vertex)
{
    vertices.push_back(vertex);
}
void Polygon::add_vertex(int x, int y)
{
    float _x, _y;
    _x = static_cast<float>(x);
    _y = static_cast<float>(y);
    Vector2D v {_x, _y};
    vertices.push_back(v);
    // vertices.emplace_back(_x, _y);
}

// Return number of vertices.
unsigned int Polygon::size()
{
    return vertices.size();
}


// Return True if a given point is inside polygon.
bool Polygon::point_in_polygon(int x, int y)
{
    /*
        Is this raycasting? e.g. casting a ray in any direction and then checking
        number of time it crosses a polygon edge?
        Even number = outside
        Odd number = inside
        We now maybe check all crossing on a vertical line going through the point.
    */
    unsigned int i;
    long long unsigned int j {vertices.size() - 1};
    bool odd_nodes {false};
    for ( i = 0; i < vertices.size(); i++ )
    {
        if ( (vertices[i].y < y && vertices[j].y >= y) || (vertices[j].y < y && vertices[i].y >= y) )
        {
            if ( vertices[i].x + (y - vertices[i].y)/(vertices[j].y - vertices[i].y)*(vertices[j].x - vertices[i].x) < x)
            {
                odd_nodes = !odd_nodes;
            }
        }
        j = i;
    }
    return odd_nodes;
}


// Scale polygon by a given value.
void Polygon::scale(float _scale)
{
    for ( Vector2D &vertex: vertices )
    {
        vertex.x *= _scale;
        vertex.y *= _scale;
    }
}


// Move polygon by a given distance.
void Polygon::move(int dx, int dy)
{
    for ( Vector2D &vertex : vertices )
    {
        vertex.x += dx;
        vertex.y += dy;
    }
}