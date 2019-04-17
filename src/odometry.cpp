#include "odometry.hpp"
#include <math.h>

Odometry::Odometry(uint32_t resolution, float center_distance,
  float wheel_diameter, float ratio, float x, float y, float theta) :
  _resolution(resolution), _center_distance(center_distance),
  _wheel_diameter(wheel_diameter), _ratio(ratio),
  _x(x), _y(y), _theta(theta) {}

void Odometry::update(float left_step, float right_step) {
  float left_distance = left_step * _wheel_diameter / _resolution;
  float right_distance = right_step * _ratio * _wheel_diameter / _resolution;

  float translation = (left_distance + right_distance) / 2;
  float rotation = (-left_distance + right_distance) / _center_distance;

  _theta += rotation;
  _x += cos(_theta) * translation;
  _y += sin(_theta) * translation;
}

float Odometry::getX() {
  return _x;
}

float Odometry::getY() {
  return _y;
}

float Odometry::getTheta() {
  return _theta;
}

void Odometry::setX(float x) {
  _x = x;
}

void Odometry::setY(float y) {
  _y = y;
}

void Odometry::setTheta(float theta) {
  _theta = theta;
}
