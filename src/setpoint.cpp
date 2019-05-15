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

  const float p_t = 0.1, d_t = 0;
  const float p_r = 1, d_r = 0.5;

  // Diminuer la fréquence de l'asservissement
  // Les distances sont trop petites pour avoir un angle correct
  // Comment augmenter ces distances
  // - diminuer p_t : ne fonctionne pas bien
  // - diminuer la fréquence de l'asservissement en position : prometteur

  // Update for straignt line
  if (delta_translation > step2cm(_error_threshold/3)) {
    _left_control->reference = _left_control->measurement
      + cm2step(delta_translation)*p_t + cm2step(delta_translation - last_delta_translation)*d_t
      - rad2step(delta_rotation)*p_r + cm2step(delta_rotation - last_delta_rotation)*d_r;
    _right_control->reference = _right_control->measurement
      + cm2step(delta_translation)*p_t + cm2step(delta_translation - last_delta_translation)*d_t
      + rad2step(delta_rotation)*p_r + cm2step(delta_rotation - last_delta_rotation)*d_r;
  }

  // Update for Orientation
  // _left_control->reference = _left_control->measurement
  //                          - 1*rad2step(delta_theta)
  //                          - 1*cm2step(delta_theta - last_delta_theta);
  // _right_control->reference = _right_control->measurement
  //                           + 1*rad2step(delta_theta)
  //                           + 1*cm2step(delta_theta - last_delta_theta);

  last_delta_theta = delta_theta;
  last_delta_translation = delta_translation;
  last_delta_rotation = delta_rotation;
}
