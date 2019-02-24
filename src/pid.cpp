#include "pid.hpp"
#include <Arduino.h>

PID::PID(float* setpoint, float* input, float* output, float kp, float ki, float kd) : _setpoint(setpoint), _input(input), _output(output), _kp(kp), _ki(ki), _kd(kd) {
  _running = 0;
  _lastInput = 0;
  _integral = 0;
  _maxOutput = 255;
  _sampleTime = 1;
}

void PID::setSampleTime(uint32_t sampleTime) {
  // float ratio = sampleTime / _sampleTime;
  // _ki *= ratio;
  // _kd /= ratio;
  _sampleTime = sampleTime;
}

void PID::setOutputLimit(float maxOutput) {
  _maxOutput = maxOutput;
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
    _derivative = *_input - _lastInput;

    /*Compute PID Output*/
    *_output = _kp * error + _ki * _integral - _kd * _derivative;

    if (*_output > _maxOutput) {
      _integral -= error;
      *_output = _maxOutput;
    }
    else if (*_output < -_maxOutput) {
      _integral -= error;
      *_output = -_maxOutput;
    }

    /*Remember some variables for next time*/
    _lastInput = *_input;
  }
}
