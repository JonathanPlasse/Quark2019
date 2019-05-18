#include "setpoint.hpp"
#include <math.h>

Setpoint::Setpoint(float error_threshold) :
_error_threshold(error_threshold), _delta_move({0, 0}), _state(STOP) {}

void Setpoint::set_current_position(const position_t* current_position) {
  _current_position = current_position;
}

void Setpoint::set_setpoint_position(const position_t* setpoint_position) {
  _setpoint_position = setpoint_position;
  _state = ORIENT;
}

delta_move_t* Setpoint::update() {
  float delta_x = _setpoint_position->x - _current_position->x;
  float delta_y = _setpoint_position->y - _current_position->y;

  if (_delta_move.delta_rotation <= 0.05) {
    _delta_move.delta_translation = 0;
    _delta_move.delta_rotation = 0;
  }

  switch(_state) {
    case STOP:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = 0;
      break;
    case ORIENT:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = pi_modulo(atan2f(delta_y, delta_x) - _current_position->theta);

      if (fabsf(_delta_move.delta_rotation) <= 0.05) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        _state = MOVE;
      }
      break;
    case MOVE:
      _delta_move.delta_translation = sqrtf(delta_x*delta_x + delta_y*delta_y);
      _delta_move.delta_rotation = pi_modulo(atan2f(delta_y, delta_x) - _current_position->theta);

      if (fabsf(_delta_move.delta_translation) <= 0.5) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        _state = TURN;
      }
      break;
    case TURN:
      _delta_move.delta_translation = 0;
      _delta_move.delta_rotation = _setpoint_position->theta - _current_position->theta;

      if (fabsf(_delta_move.delta_rotation) <= 0.05) {
        _delta_move.delta_translation = 0;
        _delta_move.delta_rotation = 0;
        _state = STOP;
      }
      break;
  }

  return &_delta_move;
}
