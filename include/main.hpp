#ifndef MAIN_HPP
#define MAIN_HPP

#include <Encoder.h>
#include "motor.hpp"
#include "rst.hpp"

// Execute function every sample_time
void timer(uint32_t time, uint32_t sample_time);

// Control the motor
void control_system();

// Execute multiple step response and send the measurement
bool step_response(Motor* motor, Encoder* encoder);

#endif
