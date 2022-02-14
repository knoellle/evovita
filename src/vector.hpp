#pragma once

class Vector2 {
public:
  float x;
  float y;

  Vector2();
  Vector2(float x, float y);

  Vector2 operator+(const Vector2 &rhs) const;
  Vector2 operator-(const Vector2 &rhs) const;
};
