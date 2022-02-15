#pragma once
#include "vector.hpp"

class Individual {
public:
  float life = 0;
  Vector2 position;
  float orientation;
  float clock_offset;

  float inputs[10];
  float outputs[2];

  Individual();
  Individual(float life, Vector2 position, float orientation);

  void process();
};
