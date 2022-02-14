#pragma once

#include <cmath>

#define M_PI 3.14159265358979323846

inline float angle_difference(float a, float b) {
  // const auto c = M_PI;
  // return c - fabs(fmod(fabs(a - b), 2 * c) - c);
  return fmod(a - b + M_PI, M_PI * 2.0) - M_PI;
}
