#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>

class PID {
public:
  //Initialisation of the PID class
  PID(float* setpoint, float* input, float* output, float kp, float ki, float kd);

  //Set sample time
  void setSampleTime(uint32_t sampleTime);

  //Set min/max output
  void setOutputLimit(float maxOutput);

  //Start the PID
  void start();

  //Stop the PID
  void stop();

  //Compute the PID and update the output
  void compute();

private:
  float *_setpoint, *_input, *_output;
  float _kp, _ki, _kd;
  uint16_t _sampleTime;
  float _maxOutput;
  uint8_t _running;
  float _lastInput, _integral, _derivative;
};

#endif
