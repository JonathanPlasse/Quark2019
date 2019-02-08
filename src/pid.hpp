#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>

class PID {
public:
  //Initialisation of the PID class
  PID(uint32_t& setpoint, uint32_t& input, uint32_t& output, uint16_t kp, uint16_t ki, uint16_t kd, uint8_t sampleTime);
  //Run the PID and update the output
  void run();

private:
  uint32_t& _setpoint, _input, _output;
  uint16_t _kp, _ki, _kd;
  uint16_t _sampleTime;
};

#endif
