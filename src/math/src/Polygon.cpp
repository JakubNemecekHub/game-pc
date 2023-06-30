#include "../polygon.hpp"

#include <vector>
#include <iostream>

#include "../managers/RenderManager.hpp"
#include "../Vector2D.hpp"
#include "../utils/SDL_GFX.hpp"


// Constructor
Polygon::Polygon(std::vector<Vector2D> _vertices)
    : vertices{_vertices} {}


Polygon::Polygon(std::vector<std::vector<float>> _vertices)
{
    for ( std::vector<float>& vertex : _vertices )
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


// Add vertex at the end of the list of vertices.
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
void Polygon::add_vertices(std::vector<std::vector<float>> vertices)
{
    for ( auto vertex : vertices )
    {
        this->vertices.emplace_back(vertex[0], vertex[1]);
    }
}


// Return number of vertices.
unsigned int Polygon::size() const
{
    return static_cast<unsigned int>(vertices.size());
}


// Return true if polygon has no vertices.
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
bool Polygon::point_in_polygon(float x, float y)
{
    /*
        Casting a ray in the -x direction and then checking
        number of time it crosses any polygon edge.
        Even number = outside
        Odd number = inside
    */
    unsigned int i;
    long long unsigned int j {vertices.size() - 1};
    bool odd_nodes {false};
    for ( i = 0; i < vertices.size(); i++ ) // using i, j to loop the polygon two points at a time
    {
        // Check if the point lies in the y-range of the two vertices
        // if not, we don't need to check the crossing with this edge
        if (
                (vertices[i].y <= y && vertices[j].y > y) ||    // This two lines check if point inside the vertices y range
                (vertices[j].y <= y && vertices[i].y > y) &&
                (vertices[j].x <= x || vertices[i].x <= y)      // Check only vertices on the left side of point
            )
        {
            if ( vertices[i].x + (y - vertices[i].y)/(vertices[j].y - vertices[i].y)*(vertices[j].x - vertices[i].x) <= x)
            {
                odd_nodes = !odd_nodes;
            }
        }
        j = i;
    }
    return odd_nodes;
}


// Scale polygon by a given value.
void Polygon::scale(float _scale, bool _in_place)
{
    // get the initial position of the firts vertex
    const float initial_x = this->x();
    const float initial_y = this->y();
    for ( Vector2D &vertex: vertices )
    {
        vertex.x *= _scale;
        vertex.y *= _scale;
    }
    // Compute deltas
    const float delta_x = initial_x - this->x();
    const float delta_y = initial_y - this->y();
    // Move polygon back
    if (_in_place) this->move(delta_x, delta_y);
}


// Move polygon by a given distance.
void Polygon::move(float dx, float dy)
{
    for ( Vector2D &vertex : vertices )
    {
        vertex.x += dx;
        vertex.y += dy;
    }
}

// Return the square of the distance
float distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return (dx*dx) + (dy*dy);
}

Vector2D* Polygon::closest_vertex(float x, float y, float threshold)
{
    int closest_index = -1;
    float closest_distance = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < vertices.size(); i++)
    {
        float next_distance = distance(vertices.at(i).x, vertices.at(i).y, x, y);
        if ( (next_distance < closest_distance) & (next_distance <= threshold) )
        {
            closest_distance = next_distance;
            closest_index = i;
        }
    }
    return ( closest_distance != -1 ) ? &vertices.at(closest_index) : nullptr;
}


// Polygons "position"
float Polygon::x()
{
    auto point = std::min_element(vertices.begin(), vertices.end(), [](auto a, auto b) { return a.x < b.x; });
    return point->x;
}
float Polygon::y()
{
    auto point = std::min_element(vertices.begin(), vertices.end(), [](auto a, auto b) { return a.y < b.y; });
    return point->y;
}
void Polygon::x(float x)
{
    float dx = x - this->x();
    move(dx, 0);
}
void Polygon::y(float y)
{
    float dy = y - this->y();
    move(0, dy);
}
 

/*
    Render scaled and moved version of polygon.
*/
void Polygon::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    size_t i, j;
    int x1, y1, x2, y2;
    j = this->size() - 1;
    for ( i = 0; i < this->size(); i++ )
    {
        x1 = static_cast<int>(vertices[j].x * visual.scale + visual.dx);
        y1 = static_cast<int>(vertices[j].y * visual.scale + visual.dy);
        x2 = static_cast<int>(vertices[i].x * visual.scale + visual.dx);
        y2 = static_cast<int>(vertices[i].y * visual.scale + visual.dy);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        gfx::square_filled(renderer, x1, y1, 6);
        j = i;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}