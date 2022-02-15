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

Vector2 &Vector2::operator+=(const Vector2 &rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
};

Vector2 &Vector2::operator-=(const Vector2 &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
};

Vector2 Vector2::operator*(const float &rhs) const {
  return Vector2(this->x * rhs, this->y * rhs);
};

Vector2 Vector2::operator/(const float &rhs) const {
  return Vector2(this->x / rhs, this->y / rhs);
};

Vector2 &Vector2::operator*=(const float &rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
};

Vector2 &Vector2::operator/=(const float &rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
};

float Vector2::norm() const { return std::sqrt(x * x + y * y); };

float Vector2::norm_squared() const { return x * x + y * y; };

float Vector2::angle() const { return std::atan2(y, x); };
SDL_FPoint Vector2::as_point() const { return SDL_FPoint{x, y}; };
