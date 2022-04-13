#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "Vector2D.hpp"


class Polygon
{
private:
public:
    std::vector<Vector2D> vertices;
    Polygon() {};
    Polygon(std::vector<Vector2D> _vertices);
    ~Polygon() {};
    void add_vertex(Vector2D vertex);
    void add_vertex(int x, int y);
    unsigned int size();
    bool point_in_polygon(int x, int y);
    void scale(float _scale);
    void move (int dx, int dy);
};

#endif // POLYGON_H
