#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>

class Motor {
public:
  // Initialisation of the Motor class
  Motor(uint8_t dir_pin1, uint8_t dir_pin2, uint8_t pwm_pin);

  // Get the pwm sent to the H bridge
  int16_t get_pwm() const;

  // Set the pwm sent to the H bridge
  void set_pwm(int16_t pwm);

private:
  uint8_t _dir_pin1, _dir_pin2, _pwm_pin;
  float _pwm;
};

#endif
