#include "../Vector2D.hpp"

#include <iostream> // ostream
#include <math.h>   // sqrt

#include "../managers/RenderManager.hpp"
#include "../utils/SDL_GFX.hpp"

// Constructor ====================================================================================
Vector2D::Vector2D(float x, float y)
    : x(x), y(y) {}

// Methods ========================================================================================
void Vector2D::zero()
{
    x = y = 0.0f;
}

bool Vector2D::is_zero() const
{
    return ( x == 0.0f ) && ( y == 0.0f );
}

float Vector2D::size() const
{
    return sqrtf(powf(x, 2) + powf(y, 2));
}

Vector2D Vector2D::unit()
{
    float this_size = this->size();
    Vector2D temp {x / this_size, y / this_size};
    return temp;
}

// Operators =======================================================================================

// Copy assignment operator
Vector2D& Vector2D::operator=(const Vector2D& vec)
{
    this->x = vec.x;
    this->y = vec.y;
    return *this;
}

// Adding a vector to a vector by operator
Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

// Subtracting a vector from vector by operator
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

// Multiplication by number (scaling)  by operator
Vector2D& Vector2D::operator*=(const float n)
{
    this->x *= n;
    this->y *= n;
    return *this;
}

// Division by number (scaling)  by operator
Vector2D& Vector2D::operator/=(const float n)
{
    this->x /= n;
    this->y /= n;
    return *this;
}

// Friend Operators ================================================================================
Vector2D operator-(const Vector2D& vec)
{
    Vector2D temp {-1.0f * vec.x, -1.0f * vec.y};
    return temp;
}

Vector2D operator+(const Vector2D& vec1, const Vector2D& vec2)
{
    Vector2D temp {vec1.x + vec2.x, vec1.y + vec2.y};
    return temp;
}

Vector2D operator-(const Vector2D& vec1, const Vector2D& vec2)
{
    Vector2D temp {vec1.x - vec2.x, vec1.y - vec2.y};
    return temp;
}

// Multiplication vector * float
Vector2D operator*(const Vector2D& vec, const float n)
{
    Vector2D temp {vec.x * n, vec.y * n};
    return temp;
}

// Multiplication float * vector
Vector2D operator*(const float n, const Vector2D& vec)
{
    Vector2D temp {vec.x * n, vec.y * n};
    return temp;
}

// Multiplication by vector (dot product)
float operator*(const Vector2D& vec1, const Vector2D& vec2)
{
    float dot_product {0};
    dot_product = (vec1.x * vec2.x) + (vec1.y * vec2.y);
    return dot_product;
}

Vector2D operator/(const Vector2D& vec, const float n)
{
    Vector2D temp {vec.x / n, vec.y / n};
    return temp;
}

// Stream Operators ================================================================================
std::ostream& operator<<(std::ostream& os, const Vector2D& vec)
{
    os << "[" << vec.x << ", " << vec.y << "]";
    return os;
}


/*
    Render scaled and moved version of vector.
*/
void Vector2D::render(SDL_Renderer* renderer, Camera camera) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    int x2 { static_cast<int>(x * camera.zoom - camera.position.x) };
    int y2 { static_cast<int>(y * camera.zoom - camera.position.y) };
    SDL_RenderDrawLine(renderer, 0, 0, x2, y2);
    gfx::square_filled(renderer, x2, y2, 6);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

