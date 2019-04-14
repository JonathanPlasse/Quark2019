#ifndef MAIN_HPP
#define MAIN_HPP

#include <Encoder.h>
#include "motor.hpp"
#include "rst.hpp"

void timer(uint32_t time, uint32_t sample_time);
void control_system();
bool step_response(Motor* motor, Encoder* encoder);

#endif
