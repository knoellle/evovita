#include "vector.hpp"

#include <cmath>

Vector2::Vector2() : x{0.0f}, y{0.0f} {};
Vector2::Vector2(float x, float y) : x{x}, y{y} {}

Vector2 Vector2::from_polar(float orientation, float length) {
  return Vector2(std::cos(orientation) * length,
                 std::sin(orientation) * length);
};

Vector2 Vector2::operator+(const Vector2 &rhs) const {
  return Vector2(this->x + rhs.x, this->y + rhs.y);
};

Vector2 Vector2::operator-(const Vector2 &rhs) const {
  return Vector2(this->x - rhs.x, this->y - rhs.y);
};

Vector2 Vector2::operator*(const float &rhs) const {
  return Vector2(this->x * rhs, this->y * rhs);
};
