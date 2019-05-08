#ifndef SETPOINT_HPP
#define SETPOINT_HPP

#include "position.hpp"
#include "control.hpp"

class Setpoint {
public:
  // Initialize Setpoint class
  Setpoint(control_t* left_control, control_t* right_control);

  // Set current_position pointer
  void set_current_position(const position_t* current_position);

  // Set setpoint_position pointer
  void set_setpoint_position(const position_t* setpoint_position);

  // Update reference
  void update();

private:
  control_t* _left_control;
  control_t* _right_control;
  const position_t* _current_position;
  const position_t* _setpoint_position;
};

#endif
