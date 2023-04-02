#include "../Vector2D.hpp"

#include <iostream> // ostream
#include <math.h>   // sqrt

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
    return static_cast<float>(sqrt(pow(x, 2) + pow(y, 2)));
}

Vector2D Vector2D::unit()
{
    float this_size = this->size();
    Vector2D temp {x / this_size, y / this_size};
    return temp;
}

// Adding a vector to a vector
Vector2D& Vector2D::add(const Vector2D& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

// Subtracting a vector from vector
Vector2D& Vector2D::subtract(const Vector2D& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

// Multiplication by number (scaling)
Vector2D& Vector2D::multiply(const float n)
{
    this->x *= n;
    this->y *= n;
    return *this;
}

// Multiplication by vector (dot product)
float Vector2D::multiply(const Vector2D& vec) const
{
    float dot_product {0};
    dot_product = (this->x * vec.x) + (this->y * vec.y);
    return dot_product;
}

// Division by number (scaling)
Vector2D& Vector2D::divide(const float n)
{
    this->x /= n;
    this->y /= n;
    return *this;
}

// Operators =======================================================================================
// Adding a vector to a vector by operator
Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    return this->add(vec);
}

// Subtracting a vector from vector by operator
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    return this->subtract(vec);
}

// Multiplication by number (scaling)  by operator
Vector2D& Vector2D::operator*=(const float n)
{
    return this->multiply(n);
}

// Division by number (scaling)  by operator
Vector2D& Vector2D::operator/=(const float n)
{
    return this->divide(n);
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
    return vec1.multiply(vec2);
}

Vector2D operator/(const Vector2D& vec, const float n)
{
    Vector2D temp {vec.x / n, vec.y / n};
    return temp;
}

// Comparison
bool operator==(const Vector2D& vec1, const Vector2D& vec2)
{
    return ( vec1.x == vec2.x ) && ( vec1.y == vec2.y );
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
void Vector2D::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    int x2 { static_cast<int>(x * visual.scale + visual.dx) };
    int y2 { static_cast<int>(y * visual.scale + visual.dy) };
    SDL_RenderDrawLine(renderer, 0, 0, x2, y2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

