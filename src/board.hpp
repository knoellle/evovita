#pragma once
#include <vector>

#include "individual.hpp"
#include "vector.hpp"

class Board {
public:
  std::vector<Individual> individuals;
  std::vector<Vector2> food;

  float simulation_time;

  void tick(float time_delta);
};
