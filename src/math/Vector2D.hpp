#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>


class Camera;


class Vector2D
{
public:
    // Fields
    float x;
    float y;
    // Methods
    void zero();
    bool is_zero() const;
    float size() const;
    Vector2D unit();

    Vector2D& add(const Vector2D& vec);
    Vector2D& add(const float x, const float y);
    Vector2D& subtract(const Vector2D& vec);
    float multiply(const Vector2D& vec) const;  // Multiplication by vector (dot product)
    Vector2D& multiply(const float n);          // Multiplication by number (scaling)
    Vector2D& divide(const float n);            // Division by number (scaling)

    // Operators
    Vector2D& operator=(const Vector2D& vec);
    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const float n);
    Vector2D& operator/=(const float n);

    friend Vector2D operator-(const Vector2D& vec);
    friend Vector2D operator+(const Vector2D& vec1, const Vector2D& vec2); // Probably cannot return reference. Might return pointer?
    friend Vector2D operator-(const Vector2D& vec1, const Vector2D& vec2);
    friend Vector2D operator*(const Vector2D& vec, const float n); // This could be member overloaded operator
    friend Vector2D operator*(const float n, const Vector2D& vec);
    friend float operator*(const Vector2D& vec1, const Vector2D& vec2);
    friend Vector2D operator/(const Vector2D& vec, const float n);
    // Comparison operators
    bool operator==(const Vector2D& vec) const { return (this->x == vec.x) && (this->y == vec.y); }
    bool operator!=(const Vector2D& vec) const { return (this->x != vec.x) || (this->y != vec.y); }
    bool operator<(const Vector2D& vec)  const { return this->size() < vec.size();                }
    bool operator>(const Vector2D& vec)  const { return this->size() > vec.size();                }
    bool operator<=(const Vector2D& vec) const { return this->size() <= vec.size();               }
    bool operator>=(const Vector2D& vec) const { return this->size() >= vec.size();               }

    // Other Methods

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
    // Constructor
    Vector2D(float x = 0.0f, float y = 0.0f);

    // Rendering methods
    void render(SDL_Renderer* renderer, Camera camera) const;
};

// YAML converter for my Vector2D class
namespace YAML
{

template<>
struct convert<Vector2D>
{

    static Node encode(const Vector2D& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        return node;
    }

    static bool decode(const Node& node, Vector2D& rhs)
    {
        if(!node.IsSequence() || node.size() != 2)
        {
            return false;
        }
        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        return true;
    }
};

};  // namespace
