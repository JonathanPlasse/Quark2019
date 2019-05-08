#include "main.hpp"
#include <Arduino.h>
#include <Encoder.h>
#include "motor.hpp"
#include "step_response.hpp"
#include "rst.hpp"
#include "control.hpp"
#include "position.hpp"
#include "odometry.hpp"
#include "binary_serial.hpp"
#include "setpoint.hpp"

// Initialization of the motors
Motor motor1(M1_DIR1, M1_DIR2, M1_PWM);
Motor motor2(M2_DIR1, M2_DIR2, M2_PWM);

// Initialization of the encoders
Encoder encoder1(ENC1_PIN1, ENC1_PIN2);
Encoder encoder2(ENC2_PIN1, ENC2_PIN2);

// Initialization of the RST setting
const uint8_t order = 2;

float r1[order+1] = {52.38595863564986, -94.7719172712997, 42.76095863564984};
float s1[order+1] = {1.0, -0.7251297263184233, -0.2748702736815767};
float t1[order+1] = {7.314144660912829, -6.939144660912829, 0.};
float r2[order+1] = {58.744438426449506, -106.42221018623232, 48.07777175978282};
float s2[order+1] = {1.0, -0.7249983334697319, -0.2750016665302681};
float t2[order+1] = {8.201666597226364, -7.8016665972263635, 0.};

float min_command = -200, max_command = 200;

float error_threshold = 10, _pwm_threshold = 150;

// Initialization of the system variables
control_t control1 = {0, 0, 0};
control_t control2 = {0, 0, 0};

// Initialization of the RST
Rst rst1(&control1, min_command, max_command,
         error_threshold, _pwm_threshold);
Rst rst2(&control2, min_command, max_command,
         error_threshold, _pwm_threshold);

// Initialization for the timer
uint8_t sample_time = 5;
uint32_t time, last_time;

// Initialization of Odometry
Odometry odometry;

// Initialization of Setpoint
position_t setpoint_position = {10, 0, 0};
Setpoint setpoint(&control1, &control2);

void setup() {
  // Change the frequency of the pwm.
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  // Set rst coefficient
  rst1.set_rst(r1, s1, t1, order);
  rst2.set_rst(r2, s2, t2, order);

  // Initialize serial connection.
  Serial.begin(115200);

  // Run the step_response
  // step_response(&motor1, &encoder1);
  // step_response(&motor2, &encoder2);

  // Set position pointer to Setpoint
  setpoint.set_current_position(odometry.get_position());
  setpoint.set_setpoint_position(&setpoint_position);
}


void loop() {
  // Execute timer
  timer(millis(), sample_time);
}


void timer(uint32_t time, uint8_t sample_time) {
  static uint32_t last_time = millis();

  if (time - last_time > sample_time) {
    // Update last_time
    last_time += sample_time;

    control_system();
  }
}


void control_system() {
  // Read motor position
  control1.measurement = encoder1.read();
  control2.measurement = encoder2.read();

  // Odometry
  odometry.update(control1.measurement, control2.measurement);

  // Debug
  // static uint8_t c = 100;
  // if (c++ == 100) {
  //   write_data(odometry.get_position(), sizeof(position_t));
  //   write_data(setpoint_position, sizeof(position_t));
  //   c = 0;
  // }

  // Update setpoint
  setpoint.update();

  // Compute control command
  rst1.compute();
  rst2.compute();

  // Apply the command on the motors
  motor1.set_pwm(control1.command);
  motor2.set_pwm(control2.command);
}
