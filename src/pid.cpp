#include "pid.hpp"

PID::PID(uint32_t& setpoint, uint32_t& input, uint32_t& output, uint16_t kp, uint16_t ki, uint16_t kd, uint8_t sampleTime) : _setpoint(setpoint), _input(input), _output(output), _kp(kp), _ki(ki), _kd(kd), _sampleTime(sampleTime) {
  _running = 0;
  _lastInput = 0;
  _integral = 0;
  _maxOutput = -1;
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
    _error = _setpoint - _input;
    _integral += _error;
    _derivative = _input - _lastInput;

    /*Compute PID Output*/
    _output = _kp * _error + _ki * _integral - _kd * _derivative;

    if (_output > _maxOutput) {
      _integral -= (_output - _maxOutput) / _ki;
      _output = _maxOutput;
    }
    else if (_output < -_maxOutput) {
      _integral += (-_maxOutput - _output) / _ki;
      _output = -_maxOutput;
    }

    /*Remember some variables for next time*/
    _lastInput = _input;
  }
}
