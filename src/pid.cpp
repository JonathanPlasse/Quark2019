#include "pid.hpp"

PID::PID(uint32_t* setpoint, uint32_t* input, uint32_t* output, float kp, float ki, float kd) : _setpoint(setpoint), _input(input), _output(output), _kp(kp), _ki(ki), _kd(kd) {
  _running = 0;
  _lastInput = 0;
  _integral = 0;
  _maxOutput = -1;
  _sampleTime = 1;
}

void PID::setSampleTime(uint32_t sampleTime) {
  float ratio = sampleTime / _sampleTime;
  _ki *= ratio;
  _kd /= ratio;
  _sampleTime = sampleTime;
}

void PID::setOutputLimit(uint32_t maxOutput) {
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
    uint32_t error = *_setpoint - *_input;
    _integral += (uint32_t) _ki * error;
    _derivative = *_input - _lastInput;

    /*Compute PID Output*/
    *_output = (uint32_t) _kp * error + _integral - _kd * _derivative;

    if (*_output > _maxOutput) {
      _integral -= (*_output - _maxOutput);
      *_output = _maxOutput;
    }
    else if (*_output < -_maxOutput) {
      _integral += (-_maxOutput - *_output);
      *_output = -_maxOutput;
    }

    /*Remember some variables for next time*/
    _lastInput = *_input;
  }
}
