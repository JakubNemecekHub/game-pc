#include "../polygon.hpp"

#include <vector>

#include "../Vector2D.hpp"


// Constructor
Polygon::Polygon(std::vector<Vector2D> _vertices)
    : vertices{_vertices} {}


Polygon::Polygon(std::vector<std::vector<int>> _vertices)
{
    for ( std::vector<int>& vertex : _vertices )
    {
        Vector2D vec {static_cast<float>(vertex[0]), static_cast<float>(vertex[1])};
        vertices.push_back(vec);
    }
}


// Copy Constructor
Polygon::Polygon(const Polygon &source)
{
    vertices = source.vertices;
}


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

// Add new vertices at the back of the existing vertices
void Polygon::add_vertices(std::vector<Vector2D> vertices)
{
    this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
}
void Polygon::add_vertices(std::vector<std::vector<int>> vertices)
{
    for ( auto vertex : vertices )
    {
        this->vertices.emplace_back(vertex[0], vertex[1]);
    }
}


// Return number of vertices.
unsigned int Polygon::size()
{
    return vertices.size();
}


// Return true if polgon has no vertices.
bool Polygon::empty()
{
    return size() == 0;
}


// Clear entire Polygon.
void Polygon::clear()
{
    vertices.clear();
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