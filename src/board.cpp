#include "board.hpp"

#include <cmath>

void Board::tick(float time_delta) {
  float view_range_sqrt = view_range * view_range;
  float eat_range_sqrt = eat_range * eat_range;
  for (auto &individual : individuals) {
    for (int i = 0; i < 10; i++)
      individual.inputs[i] *= 0.99;
    // Check for collision & sight
    for (int i = 0; i < food.size(); i++) {
      Vector2 difference_from_food = food[i] - individual.position;
      if (difference_from_food.norm_squared() < view_range_sqrt) {
        if (difference_from_food.norm_squared() < eat_range_sqrt) {
          individual.life += 1.0;
          food[i] = food[food.size() - 1];
          food.pop_back();
          i--;
        } else {
          float angle_to_food =
              std::atan2(difference_from_food.y, difference_from_food.x);
          float relative_angle =
              angle_difference(angle_to_food, individual.orientation);
          int input_index =
              std::round((relative_angle / M_PI * 2.0 + 0.5) * 10.0);
          individual.life =
              std::round((relative_angle / M_PI * 2.0 + 0.5) * 10.0);
          if (input_index >= 10 || input_index < 0) {
            LOG("===================== This should never happen "
                "======================");
            LOG("=====================   wrong input index: %d  "
                "======================",
                input_index);
            continue;
          }
          individual.inputs[input_index] += time_delta;
        }
      }
    }
    individual.process();

    // Process inputs

    // move
    // float rotation = std::copysign(
    //     1.0, angle_difference(individual.orientation,
    //                           individual.position.angle() + M_PI));
    float rotation = clamp(individual.outputs[0], -1.0, 1.0);
    individual.orientation -= rotation * time_delta * 10.0;
    individual.position += Vector2::from_polar(
        individual.orientation,
        clamp(individual.outputs[1] + 0.2, 0.0, 1.0) * time_delta * 10.0);
  }
}
