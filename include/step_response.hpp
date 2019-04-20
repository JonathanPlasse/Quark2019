#ifndef STEP_RESPONSE_HPP
#define STEP_RESPONSE_HPP

#include <stdint.h>
#include <Encoder.h>
#include "motor.hpp"

// Configuration structure for step response
typedef struct {
  uint8_t pwm;
  uint8_t sample_time;
  uint8_t nb_measure;
  uint16_t nb_sample;
  uint16_t wait_time;
} config_t;

// Motor measure
typedef struct {
  uint32_t timestamp;
  uint32_t position;
  float speed;
} measure_t;

// Execute multiple step response and send the measurement
bool step_response(Motor* motor, Encoder* encoder, uint8_t* sample_time);

#endif
