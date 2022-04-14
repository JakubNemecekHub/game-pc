#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "Vector2D.hpp"


class Polygon
{
private:
public:
    // Fields

    std::vector<Vector2D> vertices;

    // Lifecycle

    Polygon() {};
    Polygon(std::vector<Vector2D> _vertices);
    Polygon(const Polygon &source);
    ~Polygon() {};

    // Methods

    unsigned int size();
    bool empty();
    void add_vertex(Vector2D vertex);
    void add_vertex(int x, int y);
    void clear();
    bool point_in_polygon(int x, int y);
    void scale(float _scale);
    void move (int dx, int dy);
};

#endif // POLYGON_H
