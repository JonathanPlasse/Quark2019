#include "odometry.hpp"
#include <math.h>

Odometry::Odometry(uint32_t resolution, float center_distance,
  float wheel_diameter, float ratio) :
  _resolution(resolution), _center_distance(center_distance),
  _wheel_diameter(wheel_diameter), _ratio(ratio), _position({0, 0, 0}) {}

void Odometry::update(float left_step, float right_step) {
  float left_distance = left_step * _wheel_diameter / _resolution;
  float right_distance = right_step * _ratio * _wheel_diameter / _resolution;

  float translation = (left_distance + right_distance) / 2;
  float rotation = (-left_distance + right_distance) / _center_distance;

  _position.theta += rotation;
  _position.x += cos(_position.theta) * translation;
  _position.y += sin(_position.theta) * translation;
}

const position_t* Odometry::getPosition() const {
  return &_position;
}

void Odometry::setPosition(const position_t* position) {
  _position = *position;
}
