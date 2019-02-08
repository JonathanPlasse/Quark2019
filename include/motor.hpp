#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <stdint.h>
#include <Encoder.h>

class Motor {
public:
  //Initialisation of the Motor class
  Motor(uint8_t dirPin1, uint8_t dirPin2, uint8_t pwmPin, const Encoder& enc, uint16_t sampleTime);

  //Get the pwm sent to the H bridge
  int16_t getPwm();

  //Set the pwm sent to the H bridge
  void setPwm(int16_t pwm);

  //Get the position of the motor in step
  int32_t getPosition();

  //Get the speed of the motor in step/second
  int32_t getSpeed();

  //Set the speed of the motor in step/s
  void setSpeed(int16_t speed);

  //Run the control system of the speed
  //To call every sample time
  void run();

private:
  Encoder _enc;
  uint8_t _dirPin1, _dirPin2, _pwmPin;
  int16_t _pwm;
  int32_t _position, _lastPosition;
  int32_t _targetSpeed;
  int32_t _actualSpeed;
  uint8_t _sampleTime;
};

#endif
