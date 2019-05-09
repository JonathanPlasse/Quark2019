#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(control_t* left_control, control_t* right_control) :
_left_control(left_control), _right_control(right_control) {}

void Setpoint::set_current_position(const position_t* current_position) {
  _current_position = current_position;
}

void Setpoint::set_setpoint_position(const position_t* setpoint_position) {
  _setpoint_position = setpoint_position;
}

void Setpoint::update(float left_speed, float right_speed) {
  float delta_x = _setpoint_position->x - _current_position->x;
  float delta_y = _setpoint_position->y - _current_position->y;
  float delta_theta = _setpoint_position->theta - _current_position->theta;
  float delta_translation = sqrtf(delta_x*delta_x + delta_y*delta_y);
  float delta_rotation = atan2f(delta_y, delta_x);

  // Update for straignt line
  if (delta_translation > 2) {
    _left_control->reference = _left_control->measurement + cm2step(delta_translation)
      - rad2step(delta_rotation);
    _right_control->reference = _right_control->measurement + cm2step(delta_translation)
      + rad2step(delta_rotation);
  }
  // Remove rotation correction
  else if (delta_translation > 0.2) {
    _left_control->reference = _left_control->measurement + cm2step(delta_translation);
    _right_control->reference = _right_control->measurement + cm2step(delta_translation);
  }

  // Update for Orientation
  // if (delta_rotation > 0.5) {
  //   _left_control->reference = _left_control->measurement - rad2step(delta_theta);
  //   _right_control->reference = _right_control->measurement + rad2step(delta_theta);
  // }
}
