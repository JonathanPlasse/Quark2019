#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dir_pin1, uint8_t dir_pin2, uint8_t pwm_pin, uint8_t enc_pin1, uint8_t enc_pin2, uint16_t sample_time, float kp, float ki, float kd) :
_dir_pin1(dir_pin1), _dir_pin2(dir_pin2), _pwm_pin(pwm_pin) , _enc(enc_pin1, enc_pin2), _sample_time(sample_time), _pid(&_target_speed, &_actual_speed, &_pwm, kp, ki, kd) {
  pinMode(_dir_pin1, OUTPUT);
  pinMode(_dir_pin2, OUTPUT);
  analogWrite(_pwm_pin, 0);
  _pid.set_sample_time(_sample_time);
  _pid.set_output_limit(200);
  _pwm = 0;
  _position = 0;
  _last_position = 0;
  _target_speed = 0;
  _actual_speed = 0;
}

void Motor::set_sample_time(uint16_t sample_time) {
  _sample_time = sample_time;
  _pid.set_sample_time(_sample_time);
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

int32_t Motor::get_position() const {
  return _enc.read();
}

float Motor::get_target_speed() const {
  return _target_speed;
}

float Motor::get_actual_speed() const {
  return _actual_speed;
}

void Motor::compute_speed() {
  _last_position = _position;
  _position = get_position();
  _actual_speed = (_position - _last_position) * 1000. / _sample_time;
}

void Motor::set_target_speed(float speed) {
  _target_speed = speed;
}

void Motor::run() {
  //Compute the actual speed of the motor
  compute_speed();

  _pid.compute();
  set_pwm((int16_t)_pwm);
}

void Motor::stop() {
  _pid.stop();
  set_pwm(0);
}
