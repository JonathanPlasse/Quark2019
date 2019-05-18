#ifndef RAMP_HPP
#define RAMP_HPP

// Calculate the position of the robot
class Ramp {
public:
  // Initialize Ramp class
  Ramp(float max_speed, float max_acceleration);

  // Compute speed ramp
  void compute();

private:
  float _max_speed;
  float _max_acceleration;
  float _speed;
};

#endif
