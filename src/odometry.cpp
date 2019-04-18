#include "odometry.hpp"
#include <math.h>

Odometry::Odometry(uint32_t resolution, float center_distance,
  float wheel_diameter, float ratio, position_t p) :
  _resolution(resolution), _center_distance(center_distance),
  _wheel_diameter(wheel_diameter), _ratio(ratio),
  _p(p) {}

void Odometry::update(float left_step, float right_step) {
  float left_distance = left_step * _wheel_diameter / _resolution;
  float right_distance = right_step * _ratio * _wheel_diameter / _resolution;

  float translation = (left_distance + right_distance) / 2;
  float rotation = (-left_distance + right_distance) / _center_distance;

  _p.theta += rotation;
  _p.x += cos(_p.theta) * translation;
  _p.y += sin(_p.theta) * translation;
}

position_t Odometry::getPosition() const {
  return _p;
}

void Odometry::setPosition(position_t p) {
  _p = p;
}
