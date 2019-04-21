#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include <stdint.h>
#include "position.hpp"

// Calculate the position of the robot
class Odometry {
public:
  // Initialize Odometry class
  Odometry(uint32_t resolution, float center_distance, float wheel_perimeter,
    float ratio);

  // Update odometry
  void update(float left_step, float right_step);

  // Get position
  const position_t* getPosition() const;

  // Set position
  void setPosition(const position_t* position);

private:
  // Count per revolution of encoder
  uint32_t _resolution;

  // Center distance between the two wheel
  float _center_distance;

  // Wheel diameter
  float _wheel_perimeter;

  // Wheel diameter ratio (left/right)
  float _ratio;

  // Position
  position_t _position;
};

#endif
