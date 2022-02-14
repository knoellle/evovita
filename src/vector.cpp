#include "vector.hpp"

Vector2::Vector2() : x{0.0f}, y{0.0f} {};
Vector2::Vector2(float x, float y) : x{x}, y{y} {}

Vector2 Vector2::operator+(const Vector2 &rhs) const {
  return Vector2(this->x + rhs.x, this->y + rhs.y);
};

Vector2 Vector2::operator-(const Vector2 &rhs) const {
  return Vector2(this->x - rhs.x, this->y - rhs.y);
};
