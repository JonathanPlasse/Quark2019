#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>

class PID {
public:
  //Initialisation of the PID class
  PID(int32_t* setpoint, int32_t* input, int32_t* output, float kp, float ki, float kd);

  //Set sample time
  void setSampleTime(uint32_t sampleTime);

  //Set min/max output
  void setOutputLimit(uint32_t maxOutput);

  //Start the PID
  void start();

  //Stop the PID
  void stop();

  //Compute the PID and update the output
  void compute();

private:
  int32_t *_setpoint, *_input, *_output;
  float _kp, _ki, _kd;
  uint16_t _sampleTime;

  int32_t _maxOutput;

  uint8_t _running;

  int32_t _lastInput, _integral, _derivative;
};

#endif
