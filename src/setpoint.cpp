#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(float* left_control, float* right_control,
const float* left_measurement, const float* right_measurement) :
_left_control(left_control), _right_control(right_control),
_left_measurement(left_measurement), _right_measurement(right_measurement) {}

void Setpoint::set_current_position(const position_t* current_position) {
  _current_position = current_position;
}

void Setpoint::set_setpoint_position(const position_t* setpoint_position) {
  _setpoint_position = setpoint_position;
}

void Setpoint::update() {
  float delta_x = _setpoint_position->x - _current_position->x;
  float delta_y = _setpoint_position->y - _current_position->y;
  float delta_theta = _setpoint_position->theta - _current_position->theta;
  float delta_translation = sqrtf(delta_x*delta_x + delta_y*delta_y);
  float delta_rotation = atan2f(delta_y, delta_x);

  if (delta_translation > 2) {
    *_left_control = *_left_measurement + cm2step(delta_translation)
      - rad2step(delta_rotation);
    *_right_control = *_right_measurement + cm2step(delta_translation)
      + rad2step(delta_rotation);
  }
  else {
    *_left_control += 0;
    *_right_control += 0;
  }
}
