#ifndef SETPOINT_HPP
#define SETPOINT_HPP

#include "position.hpp"

class Setpoint {
public:
  // Initialize Setpoint class
  Setpoint(float* left_control, float* right_control);

  // Set current_position pointer
  void set_current_position(const position_t* current_position);

  // Set setpoint_position pointer
  void set_setpoint_position(const position_t* setpoint_position);

  // Update control
  void update();

private:
  float* _left_control;
  float* _right_control;
  const position_t* _current_position;
  const position_t* _setpoint_position;
};

#endif
