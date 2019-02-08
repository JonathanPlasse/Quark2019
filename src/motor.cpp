#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, const Encoder& enc, uint16_t sampleTime) :
_dirPin1(dirPin1), _dirPin2(dirPin2), _pwmPin(pwmPin) , _enc(enc), _sampleTime(sampleTime) {
  pinMode(_dirPin1, OUTPUT);
  pinMode(_dirPin2, OUTPUT);
  analogWrite(_pwmPin, 0);
}

int16_t Motor::getPwm() {
  return _pwm;
}

void Motor::setPwm(int16_t pwm) {
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

int32_t Motor::getPosition() {
  return _enc.read();
}

int32_t Motor::getSpeed() {
  return _targetSpeed;
}

void Motor::setSpeed(int16_t speed) {
  _targetSpeed = speed;
}

void Motor::run() {
  //Compute the actual speed of the motor
  _lastPosition = _position;
  _position = getPosition();
  _actualSpeed = (_position - _lastPosition) / _sampleTime;


}
