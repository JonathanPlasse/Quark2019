#include "pid.hpp"
#include <Arduino.h>

PID::PID(float* setpoint, float* input, float* output, float kp, float ki, float kd) : _setpoint(setpoint), _input(input), _output(output), _kp(kp), _ki(ki), _kd(kd) {
  _running = 0;
  _last_input = 0;
  _integral = 0;
  _max_output = 255;
  _sample_time = 1;
}

void PID::set_sample_time(uint32_t sample_time) {
  // float ratio = sample_time / _sample_time;
  // _ki *= ratio;
  // _kd /= ratio;
  _sample_time = sample_time;
}

void PID::set_output_limit(float max_output) {
  _max_output = max_output;
}

void PID::start() {
  _running = 1;
}

void PID::stop() {
  _running = 0;
}

void PID::compute() {
  if (_running) {
    /*Compute all the working error variables*/
    float error = *_setpoint - *_input;
    _integral += error;
    _derivative = *_input - _last_input;

    /*Compute PID Output*/
    *_output = _kp * error + _ki * _integral - _kd * _derivative;

    if (*_output > _max_output) {
      _integral -= error;
      *_output = _max_output;
    }
    else if (*_output < -_max_output) {
      _integral -= error;
      *_output = -_max_output;
    }

    /*Remember some variables for next time*/
    _last_input = *_input;
  }
}
