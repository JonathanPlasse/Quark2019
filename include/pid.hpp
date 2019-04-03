#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>

class PID {
public:
  //Initialisation of the PID class
  PID(float* setpoint, float* input, float* output, float kp, float ki, float kd);

  //Set sample time
  void set_sample_time(uint32_t sample_time);

  //Set min/max output
  void set_output_limit(float max_output);

  //Start the PID
  void start();

  //Stop the PID
  void stop();

  //Compute the PID and update the output
  void compute();

private:
  float *_setpoint, *_input, *_output;
  float _kp, _ki, _kd;
  uint16_t _sample_time;
  float _max_output;
  uint8_t _running;
  float _last_input, _integral, _derivative;
};

#endif
