#include "main.hpp"
#include <Arduino.h>
#include <Encoder.h>
#include "motor.hpp"
#include "step_response.hpp"
#include "rst.hpp"

// Initialization of the motors
Motor motor1(M1_DIR1, M1_DIR2, M1_PWM);
Motor motor2(M2_DIR1, M2_DIR2, M2_PWM);

// Initialization of the encoders
Encoder encoder1(ENC1_PIN1, ENC1_PIN2);
Encoder encoder2(ENC2_PIN1, ENC2_PIN2);

// Initialization of the RST setting
const uint8_t order = 2;
float r1[order+1] = {17.66, -31.48, 14.01};
float s1[order+1] = {1., -0.7265 -0.2735};
float t1[order+1] = {2.466, -2.275, 0};
float r2[order+1] = {19.94, -35.67, 15.94};
float s2[order+1] = {1., -0.7262, -0.2738};
float t2[order+1] = {2.784, -2.587, 0};
float min_control = -255, max_control = 255;

// Initialization of the system variables
float reference1 = 1633*10, measurement1, control1;
float reference2 = 1633*10, measurement2, control2;

// Initialization of the RST
Rst rst1(&reference1, &measurement1, &control1, min_control, max_control);
Rst rst2(&reference2, &measurement2, &control2, min_control, max_control);

// Initialization for the timer
uint32_t sample_time = 10;
uint32_t time, last_time;


void setup() {
  // Change the frequency of the pwm.
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  // Set rst coefficient
  rst1.set_rst(r1, s1, t1, order);
  rst2.set_rst(r2, s2, t2, order);

  // Initialize serial connection.
  Serial.begin(115200);
}


void loop() {
  // Execute timer
  timer(millis(), sample_time);
}


void timer(uint32_t time, uint32_t sample_time) {
  static uint32_t last_time = millis();

  if (time - last_time > sample_time) {
    // Run the step_response
    // step_response(&motor1, &encoder1, &sample_time);
    // step_response(&motor2, &encoder2, &sample_time);
    control_system();
  }
}


void control_system() {
  // Update last_time
  last_time += sample_time;

  // Read motor position
  measurement1 = encoder1.read();
  measurement2 = encoder2.read();

  // Compute control command
  rst1.compute();
  rst2.compute();

  // Apply the control on the motors
  motor1.set_pwm(control1);
  motor2.set_pwm(control2);
}
