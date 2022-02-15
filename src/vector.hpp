#pragma once

#include <SDL2/SDL_rect.h>

class Vector2 {
public:
  float x;
  float y;

  Vector2();
  Vector2(float x, float y);
  static Vector2 from_polar(float orientation, float length);

  Vector2 operator+(const Vector2 &rhs) const;
  Vector2 operator-(const Vector2 &rhs) const;
  Vector2 &operator+=(const Vector2 &rhs);
  Vector2 &operator-=(const Vector2 &rhs);

  Vector2 operator*(const float &rhs) const;
  Vector2 operator/(const float &rhs) const;
  Vector2 &operator*=(const float &rhs);
  Vector2 &operator/=(const float &rhs);

  float norm() const;
  float norm_squared() const;
  float angle() const;
  SDL_FPoint as_point() const;
};
