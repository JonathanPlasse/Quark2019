#ifndef SETPOINT_HPP
#define SETPOINT_HPP

#include "position.hpp"
#include "control.hpp"

class Setpoint {
public:
  // Initialize Setpoint class
  Setpoint(float error_threshold);

  // Set current_position pointer
  void set_current_position(const position_t* current_position);

  // Set setpoint_position pointer
  void set_setpoint_position(const position_t* setpoint_position);

  // Update reference
  delta_move_t* update();

private:
  float _error_threshold;
  const position_t* _current_position;
  const position_t* _setpoint_position;
  delta_move_t _delta_move;
};

#endif
