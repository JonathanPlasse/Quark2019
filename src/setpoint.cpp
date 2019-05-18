#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(control_t* left_control, control_t* right_control,
                   float error_threshold, float sample_time) :
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

  // Problème : oscillation du robot et instabilité
  // Les distances sont trop petites pour avoir un angle correct
  // Comment augmenter ces distances ?
  // - diminuer p_t : ne fonctionne pas bien
  // - diminuer la fréquence de l'asservissement en position : prometteur
  // Comment réduire les oscillation ?
  // - implanter rampe de vitesse

  // PD coefficients for translation
  const float p_t = 1, d_t = 0;
  // PD coefficients for rotation
  const float p_r = 1, d_r = 0;

  // Speed ramp constants
  const float speed_max = cm2step(40), acceleration_max = cm2step(20);

  // Calculate translation and rotation speed
  float translation_speed = (left_speed+right_speed)/2;
  float rotation_speed = (-left_speed+right_speed)/2;

  float translation_brake_distance = powf(translation_speed, 2)/(2*acceleration_max);
  float rotation_brake_distance = powf(rotation_speed, 2)/(2*acceleration_max);

  // Apply translation speed ramp
  if (delta_translation < translation_brake_distance) {
    delta_translation = (translation_speed-acceleration_max*_sample_time)*_sample_time;
  }
  else if (translation_speed < speed_max) {
    delta_translation = (translation_speed+acceleration_max*_sample_time)*_sample_time;
  }
  else {
    delta_translation = speed_max*_sample_time;
  }
  // Apply rotation speed ramp
  if (delta_rotation < rotation_brake_distance) {
    delta_rotation = (rotation_speed-acceleration_max*_sample_time)*_sample_time;
  }
  else if (rotation_speed < speed_max) {
    delta_rotation = (rotation_speed+acceleration_max*_sample_time)*_sample_time;
  }
  else {
    delta_rotation = speed_max*_sample_time;
  }

  // Update for straignt line
  // if (delta_translation > step2cm(_error_threshold/3)) {
    _left_control->reference = _left_control->measurement
      + cm2step(delta_translation)*p_t + cm2step(delta_translation - last_delta_translation)*d_t
      - rad2step(delta_rotation)*p_r + cm2step(delta_rotation - last_delta_rotation)*d_r;
    _right_control->reference = _right_control->measurement
      + cm2step(delta_translation)*p_t + cm2step(delta_translation - last_delta_translation)*d_t
      + rad2step(delta_rotation)*p_r + cm2step(delta_rotation - last_delta_rotation)*d_r;
  // }

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
