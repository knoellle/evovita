#include "board.hpp"

#include <cmath>

#include "utils.hpp"

void Board::tick(float time_delta) {
  for (auto &individual : individuals) {
    float rotation = std::copysign(
        1.0, angle_difference(individual.orientation,
                              individual.position.angle() + M_PI));
    individual.orientation -= rotation * time_delta;
    individual.position +=
        Vector2::from_polar(individual.orientation, time_delta * 10.0);
  }
}
