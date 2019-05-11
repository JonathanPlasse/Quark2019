#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(control_t* left_control, control_t* right_control,
                   float error_threshold) :
_left_control(left_control), _right_control(right_control),
_error_threshold(error_threshold) {}

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
  static float last_delta_theta = 0;
  static float last_delta_translation = 0;
  static float last_delta_rotation = 0;

  // Update for straignt line
  // if (delta_translation > step2cm(_error_threshold/3)) {
  //   _left_control->reference = _left_control->measurement
  //     + cm2step(delta_translation) + cm2step(delta_translation - last_delta_translation)*0;
  //     // - rad2step(delta_rotation) / 1 + cm2step(delta_rotation - last_delta_rotation)/1;
  //   _right_control->reference = _right_control->measurement
  //     + cm2step(delta_translation) + cm2step(delta_translation - last_delta_translation)*0;
  //     // + rad2step(delta_rotation) / 1 + cm2step(delta_rotation - last_delta_rotation)/1;
  // }

  // Update for Orientation
  _left_control->reference = _left_control->measurement
                           - 1*rad2step(delta_theta)
                           - 0*cm2step(delta_theta - last_delta_theta);
  _right_control->reference = _right_control->measurement
                            + 1*rad2step(delta_theta)
                            + 1*cm2step(delta_theta - last_delta_theta);

  last_delta_theta = delta_theta;
  last_delta_translation = delta_translation;
  last_delta_rotation = delta_rotation;
}
