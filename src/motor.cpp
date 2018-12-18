#include "motor.hpp"
#include <Arduino.h>

Motor::Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, const Encoder& enc) :
_dirPin1(dirPin1), _dirPin2(dirPin2), _pwmPin(pwmPin) , _enc(enc) {
  pinMode(_dirPin1, OUTPUT);
  pinMode(_dirPin2, OUTPUT);
  analogWrite(_pwmPin, 0);
}

void Motor::setPwm(int16_t pwm) {
  if(pwm>0){
    digitalWrite(_dirPin1, LOW);
    digitalWrite(_dirPin2, HIGH);
    analogWrite(_pwmPin, pwm);
  }
  else{
    digitalWrite(_dirPin1, HIGH);
    digitalWrite(_dirPin2, LOW);
    analogWrite(_pwmPin, -pwm);
  }
  _pwm = pwm;
}

int16_t Motor::getPwm() {
  return _pwm;
}

int32_t Motor::getPosition() {
  return _enc.read();
}

int32_t Motor::getSpeed(uint32_t deltaT){
  _lastPosition = _position;
  _position = getPosition();
  return (_position - _lastPosition) / deltaT;
}
