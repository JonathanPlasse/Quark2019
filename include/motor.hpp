#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>
#include <Encoder.h>
#include "pid.hpp"

class Motor {
public:
  //Initialisation of the Motor class
  Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, uint8_t encPin1, uint8_t encPin2, uint16_t sampleTime, float kp, float ki, float kd);

  //Set sample time
  void setSampleTime(uint16_t sampleTime);

  //Get the pwm sent to the H bridge
  int16_t getPwm() const;

  //Set the pwm sent to the H bridge
  void setPwm(int16_t pwm);

  //Get the position of the motor in steps
  int32_t getPosition() const;

  //Get the target speed of the motor in steps/seconds
  float getTargetSpeed() const;

  //Get the speed of the motor in step/seconds
  float getActualSpeed() const;

  //Set the speed of the motor in steps/seconds
  void setSpeed(float speed);

  //Compute the actual speed
  void computeSpeed();

  //Run the control system of the speed
  //To call every sample time
  void run();

  //Stop the motor and the control system
  void stop();

private:
  Encoder _enc;
  uint8_t _dirPin1, _dirPin2, _pwmPin;
  float _pwm;
  int32_t _position, _lastPosition;
  float _targetSpeed;
  float _actualSpeed;
  uint16_t _sampleTime;

public:
  PID _pid;
};

#endif
