#include "odometry.hpp"
#include <math.h>

Odometry::Odometry() : _position({0, 0, 0}) {}

void Odometry::update(float left_step, float right_step) {
  float left_distance = left_step * wheel_perimeter / resolution;
  float right_distance = right_step * wheel_ratio * wheel_perimeter / resolution;

  float translation = (left_distance + right_distance) / 2;
  float rotation = (-left_distance + right_distance) / center_distance;

  _position.theta += rotation;
  _position.theta = pi_modulo(_position.theta);
  _position.x += cos(_position.theta) * translation;
  _position.y += sin(_position.theta) * translation;
}

const position_t* Odometry::getPosition() const {
  return &_position;
}

void Odometry::setPosition(const position_t* position) {
  _position = *position;
}
