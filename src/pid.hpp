#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>

class PID {
public:
  //Initialisation of the PID class
  PID(uint32_t& setpoint, uint32_t& input, uint32_t& output, uint16_t kp, uint16_t ki, uint16_t kd, uint8_t sampleTime);

  //Set min/max output
  void setOutputLimit(uint32_t maxOutput);

  //Start the PID
  void start();

  //Stop the PID
  void stop();

  //Compute the PID and update the output
  void compute();

private:
  uint32_t& _setpoint, _input, _output;
  uint16_t _kp, _ki, _kd;
  uint16_t _sampleTime;

  uint32_t _maxOutput;

  uint8_t _running;

  uint32_t _error, _lastInput, _integral, _derivative;
};

#endif
