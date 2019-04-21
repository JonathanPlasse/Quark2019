#include "setpoint.hpp"

Setpoint::Setpoint(float* left_control, float* right_control) :
_left_control(left_control), _right_control(right_control) {}

void Setpoint::set_current_position(const position_t* current_position) {
  _current_position = current_position;
}

void Setpoint::set_setpoint_position(const position_t* setpoint_position) {
  _setpoint_position = setpoint_position;
}

void Setpoint::update() {

}
