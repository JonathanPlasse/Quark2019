#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>
#include <Encoder.h>

class Motor {
public:
  //Initialisation of the Motor class
  Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, const Encoder& enc);
  //Set the pwm sent to the H bridge
  void setPwm(int16_t pwm);
  //Get the pwm sent to the H bridge
  int16_t getPwm();
  //Get the position of the motor in step
  int32_t getPosition();
  //Get the speed of the motor in step/second
  //deltaT is the time since the last call of the function
  int32_t getSpeed(uint32_t deltaT);

private:
  Encoder _enc;
  uint8_t _dirPin1, _dirPin2, _pwmPin;
  int16_t _pwm;
  int32_t _position, _lastPosition;
};

#endif
