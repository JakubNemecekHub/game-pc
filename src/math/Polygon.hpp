#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "Vector2D.hpp"
#include "Visual.hpp"

typedef std::vector<std::vector<float>> Vertices;

class Polygon
{
private:
public:
    // Fields

    std::vector<Vector2D> vertices;

    // Lifecycle

    Polygon() {};                                       // No arg constructor
    Polygon(std::vector<Vector2D> _vertices);           // Constructor with given vertices as Vector2D
    Polygon(std::vector<std::vector<int>> _vertices);   // onstructor with given vertices as vector of ints
    Polygon(const Polygon &source);                     // Copy constructor
    ~Polygon() {};                                      // Destructor

    // Methods

    unsigned int size() const;
    bool empty();
    void add_vertex(Vector2D vertex);
    void add_vertex(int x, int y);
    void add_vertices(std::vector<Vector2D> vertices);
    void add_vertices(std::vector<std::vector<float>> vertices);
    void clear();
    bool point_in_polygon(float x, float y);
    void scale(float _scale, bool _in_place = false);
    void move (float dx, float dy);

    // "Position" of a polygon is a top left corner of its envelope rectangle
    float x();
    float y();
    void x(float x);
    void y(float y);

    // Rendering methods
    Visual visual;
    void render(SDL_Renderer* renderer) const;
    
};
