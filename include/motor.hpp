#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>
#include <Encoder.h>
#include "pid.hpp"

class Motor {
public:
  //Initialisation of the Motor class
  Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, const Encoder& enc, uint16_t sampleTime, float kp, float ki, float kd);

  //Get the pwm sent to the H bridge
  int32_t getPwm() const;

  //Set the pwm sent to the H bridge
  void setPwm(int32_t pwm);

  //Get the position of the motor in step
  int32_t getPosition() const;

  //Get the speed of the motor in step/second
  int32_t getSpeed() const;

  //Set the speed of the motor in step/s
  void setSpeed(int32_t speed);

  //Run the control system of the speed
  //To call every sample time
  void run();

private:
  Encoder _enc;
  uint8_t _dirPin1, _dirPin2, _pwmPin;
  int32_t _pwm;
  int32_t _position, _lastPosition;
  int32_t _targetSpeed;
  int32_t _actualSpeed;
  uint16_t _sampleTime;
  PID _pid;
};

#endif
