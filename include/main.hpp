#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdint.h>

// Left Motor
#define M1_DIR1 A0
#define M1_DIR2 A1
#define M1_PWM 9

// Left Encoder
#define ENC1_PIN1 2
#define ENC1_PIN2 4

// Right Motor
#define M2_DIR1 A2
#define M2_DIR2 A3
#define M2_PWM 10

// Right Encoder
#define ENC2_PIN1 3
#define ENC2_PIN2 5

// Execute function every sample_time
void timer(uint32_t time, uint32_t sample_time);

// Control the motor
void control_system();

#endif
