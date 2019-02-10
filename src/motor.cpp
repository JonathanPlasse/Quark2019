#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, uint8_t encPin1, uint8_t encPin2, uint16_t sampleTime, float kp, float ki, float kd) :
_dirPin1(dirPin1), _dirPin2(dirPin2), _pwmPin(pwmPin) , _enc(encPin1, encPin2), _sampleTime(sampleTime), _pid(&_targetSpeed, &_actualSpeed, &_pwm, kp, ki, kd) {
  pinMode(_dirPin1, OUTPUT);
  pinMode(_dirPin2, OUTPUT);
  analogWrite(_pwmPin, 0);
  _pid.setSampleTime(_sampleTime);
  _pid.setOutputLimit(200);
  _pid.start();
  _pwm = 0;
  _position = 0;
  _lastPosition = 0;
  _targetSpeed = 0;
  _actualSpeed = 0;
}

int32_t Motor::getPwm() const {
  return _pwm;
}

void Motor::setPwm(int32_t pwm) {
  if (pwm > 0) {
    digitalWrite(_dirPin1, LOW);
    digitalWrite(_dirPin2, HIGH);
    analogWrite(_pwmPin, pwm);
  }
  else {
    digitalWrite(_dirPin1, HIGH);
    digitalWrite(_dirPin2, LOW);
    analogWrite(_pwmPin, -pwm);
  }
  _pwm = pwm;
}

int32_t Motor::getPosition() const {
  return _enc.read();
}

int32_t Motor::getTargetSpeed() const {
  return _targetSpeed;
}

int32_t Motor::getActualSpeed() const {
  return _actualSpeed;
}

void Motor::setSpeed(int32_t speed) {
  _targetSpeed = speed;
}

void Motor::run() {
  //Compute the actual speed of the motor
  _lastPosition = _position;
  _position = getPosition();
  _actualSpeed = (_position - _lastPosition) * 1000 / _sampleTime;

  _pid.compute();
  setPwm(_pwm);
}
