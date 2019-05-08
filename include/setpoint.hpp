#ifndef SETPOINT_HPP
#define SETPOINT_HPP

#include "position.hpp"

class Setpoint {
public:
  // Initialize Setpoint class
  Setpoint(float* left_reference, float* right_reference,
    const float* left_measurement, const float* right_measurement);

  // Set current_position pointer
  void set_current_position(const position_t* current_position);

  // Set setpoint_position pointer
  void set_setpoint_position(const position_t* setpoint_position);

  // Update reference
  void update();

private:
  float* _left_reference;
  float* _right_reference;
  const float* _left_measurement;
  const float* _right_measurement;
  const position_t* _current_position;
  const position_t* _setpoint_position;
};

#endif
