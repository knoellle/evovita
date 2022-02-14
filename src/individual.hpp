#pragma once
#include "vector.hpp"

class Individual {
public:
  float life = 0;
  Vector2 position;
  float orientation;
  float clock_offset;

  Individual();
  Individual(float life, Vector2 position, float orientation);
};
