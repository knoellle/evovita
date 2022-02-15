#include "individual.hpp"

Individual::Individual() : life{1.0}, position{0.0, 0.0}, orientation{0.0} {};

Individual::Individual(float life, Vector2 position, float orientation)
    : life{life}, position{position}, orientation{orientation} {};
;

void Individual::process() {
  outputs[0] = 0;
  outputs[1] = 1.0;
  for (int i = 0; i < 10; i++) {
    outputs[0] += inputs[i] * (static_cast<float>(i) - 4.5);
    outputs[1] -= abs(inputs[i] * (static_cast<float>(i) - 4.5));
  }
  outputs[0] /= 10.0;
  outputs[1] /= 10.0;
};
