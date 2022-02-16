#pragma once
#include <vector>

#include "individual.hpp"
#include "vector.hpp"
#include "utils.hpp"

class Board {
public:
  // Settings
  float view_range = 3.0;
  float eat_range = 0.5;

  std::vector<Individual> individuals;
  std::vector<Vector2> food;

  float simulation_time;

  void tick(float time_delta);
};
