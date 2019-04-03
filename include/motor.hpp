#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>
#include <Encoder.h>
#include "pid.hpp"

class Motor {
public:
  // Initialisation of the Motor class
  Motor(uint8_t dir_pin1, uint8_t dir_pin2, uint8_t pwm_pin, uint8_t enc_pin1, uint8_t enc_pin2, uint16_t sample_time, float kp, float ki, float kd);

  // Set sample time
  void set_sample_time(uint16_t sample_time);

  // Get the pwm sent to the H bridge
  int16_t get_pwm() const;

  // Set the pwm sent to the H bridge
  void set_pwm(int16_t pwm);

  // Get the position of the motor in steps
  int32_t get_position() const;

  // Get the target speed of the motor in steps/seconds
  float get_target_speed() const;

  // Get the actual speed of the motor in steps/seconds
  float get_actual_speed() const;

  // Set the target speed of the motor in steps/seconds
  void set_target_speed(float speed);

  // Compute the actual speed
  void compute_speed();

  // Run the control system of the speed
  //To call every sample time
  void run();

  // Stop the motor and the control system
  void stop();

private:
  Encoder _enc;
  uint8_t _dir_pin1, _dir_pin2, _pwm_pin;
  float _pwm;
  int32_t _position, _last_position;
  float _target_speed;
  float _actual_speed;
  uint16_t _sample_time;

public:
  PID _pid;
};

#endif
