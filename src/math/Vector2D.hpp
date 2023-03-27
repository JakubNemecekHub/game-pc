#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "Visual.hpp"


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
    Vector2D& subtract(const Vector2D& vec);
    float multiply(const Vector2D& vec) const;  // Multiplication by vector (dot product)
    Vector2D& multiply(const float n);          // Multiplication by number (scaling)
    Vector2D& divide(const float n);            // Division by number (scaling)

    // Operators
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
    friend bool operator==(const Vector2D& vec1, const Vector2D& vec2);

    // Other Methods

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
    // Constructor
    Vector2D(float x = 0.0f, float y = 0.0f);

    // Rendering methods
    Visual visual;
    void render(SDL_Renderer* renderer);
};
