#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dir_pin1, uint8_t dir_pin2, uint8_t pwm_pin) :
_dir_pin1(dir_pin1), _dir_pin2(dir_pin2), _pwm_pin(pwm_pin) , _pwm(0) {
  pinMode(_dir_pin1, OUTPUT);
  pinMode(_dir_pin2, OUTPUT);
  analogWrite(_pwm_pin, 0);
}

int16_t Motor::get_pwm() const {
  return (int16_t)_pwm;
}

void Motor::set_pwm(int16_t pwm) {
  if (pwm > 0) {
    digitalWrite(_dir_pin1, LOW);
    digitalWrite(_dir_pin2, HIGH);
    analogWrite(_pwm_pin, pwm);
  }
  else {
    digitalWrite(_dir_pin1, HIGH);
    digitalWrite(_dir_pin2, LOW);
    analogWrite(_pwm_pin, -pwm);
  }
  _pwm = (float)pwm;
}
