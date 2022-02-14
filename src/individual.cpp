#include "individual.hpp"

Individual::Individual() : life{1.0}, position{0.0, 0.0}, orientation{0.0} {};

Individual::Individual(float life, Vector2 position, float orientation)
    : life{life}, position{position}, orientation{orientation} {};
;
