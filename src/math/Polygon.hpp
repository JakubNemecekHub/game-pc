#pragma once

#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "Vector2D.hpp"

class Camera;

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
    Polygon(std::vector<std::vector<float>> _vertices); // Constructor with given vertices as vector of ints
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
    bool point_in_polygon(Vector2D position);
    void scale(float _scale, bool _in_place = false);
    void move (Vector2D direction);

    Vector2D* closest_vertex(float x, float y, float threshold);

    // "Position" of a polygon is a top left corner of its envelope rectangle
    float x();
    float y();
    void position(Vector2D position);
    void x(float x);
    void y(float y);

    // Rendering methods
    void render(SDL_Renderer* renderer, Camera camera) const;

};

// YAML converter for my Polygon class
namespace YAML
{

template<>
struct convert<Polygon>
{

    static Node encode(const Polygon& rhs)
    {
        Node node;
        for(const auto& vertex : rhs.vertices)
        {
            node.push_back(vertex);
        }
        return node;
    }

    static bool decode(const Node& node, Polygon& rhs)
    {
        if (!node.IsSequence()) return false;

        for (const auto& vertexNode : node)
        {
            rhs.add_vertex(vertexNode.as<Vector2D>());
        }
        return true;
    }

};

};  // namespace
