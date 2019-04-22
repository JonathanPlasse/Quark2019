#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include <stdint.h>
#include "position.hpp"

// Calculate the position of the robot
class Odometry {
public:
  // Initialize Odometry class
  Odometry();

  // Update odometry
  void update(float left_step, float right_step);

  // Get position
  const position_t* getPosition() const;

  // Set position
  void setPosition(const position_t* position);

private:
  // Position
  position_t _position;
};

#endif
