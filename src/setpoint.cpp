#include "setpoint.hpp"
#include <math.h>
#include "binary_serial.hpp"

Setpoint::Setpoint(float* left_reference, float* right_reference,
const float* left_measurement, const float* right_measurement) :
_left_reference(left_reference), _right_reference(right_reference),
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

  // Debug
  // static u int8_t c = 50;
  // if (c++ == 50) {
  //   write_data(_current_position, sizeof(position_t));
  //   write_data(&delta_translation, sizeof(float));
  //   write_data(&delta_rotation, sizeof(float));
  //   write_data(_left_measurement, sizeof(float));
  //   write_data(_right_measurement, sizeof(float));
  //   write_data(_left_reference, sizeof(float));
  //   write_data(_right_reference, sizeof(float));
  //   c = 0;
  // }


  if (delta_translation > 2) {
    *_left_reference = *_left_measurement + cm2step(delta_translation)
      - rad2step(delta_rotation);
    *_right_reference = *_right_measurement + cm2step(delta_translation)
      + rad2step(delta_rotation);
  }
}
