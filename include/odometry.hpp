#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include <stdint.h>

// Calculate the position of the robot
class Odometry {
public:
  // Initialisation of the Odometry class
  Odometry(uint32_t resolution, float center_distance, float wheel_diameter, float ratio, float x = 0, float y = 0, float theta = 0);

  // Update odometry
  void update(float left_step, float right_step);

  // Get x
  float getX();

  // Get y
  float getY();

  // Get theta
  float getTheta();

  // Set x
  void setX(float x);

  // Set y
  void setY(float y);

  // Set theta
  void setTheta(float theta);

private:
  // Count per revolution of encoder
  uint32_t _resolution;

  // Center distance between the two wheel
  float _center_distance;

  // Wheel diameter
  float _wheel_diameter;

  // Wheel diameter ratio (left/right)
  float _ratio;

  // Position
  float _x, _y, _theta;
};

#endif
