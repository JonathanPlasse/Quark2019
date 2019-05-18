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
#include "ramp.hpp"

// Initialization of the motors
Motor left_motor(M1_DIR1, M1_DIR2, M1_PWM);
Motor right_motor(M2_DIR1, M2_DIR2, M2_PWM);

// Initialization of the encoders
Encoder left_encoder(ENC1_PIN1, ENC1_PIN2);
Encoder right_encoder(ENC2_PIN1, ENC2_PIN2);

// Initialization of the RST setting
const uint8_t order = 2;

float r1[order+1] = {52.38595863564986, -94.7719172712997, 42.76095863564984};
float s1[order+1] = {1.0, -0.7251297263184233, -0.2748702736815767};
float t1[order+1] = {7.314144660912829, -6.939144660912829, 0.};
float r2[order+1] = {58.744438426449506, -106.42221018623232, 48.07777175978282};
float s2[order+1] = {1.0, -0.7249983334697319, -0.2750016665302681};
float t2[order+1] = {8.201666597226364, -7.8016665972263635, 0.};

float min_command = -200, max_command = 200;

float error_threshold = 0, pwm_threshold = 150;

// Initialization of the system variables
control_t left_control = {0, 0, 0}, last_left_control = {0, 0, 0};
control_t right_control = {0, 0, 0}, last_right_control = {0, 0, 0};


// Initialization of the RST
Rst left_rst(&left_control, min_command, max_command,
         error_threshold, pwm_threshold);
Rst right_rst(&right_control, min_command, max_command,
         error_threshold, pwm_threshold);

// Initialization for the timer
uint8_t sample_time = 5;
uint32_t time, last_time;

// Initialization of Odometry
Odometry odometry;

// Initialization of Setpoint
position_t setpoint_position = {20, 20, 0};
delta_move_t* delta_move;
Setpoint setpoint(error_threshold);

Ramp translation_ramp(100, 100, sample_time/1000.);
Ramp rotation_ramp(10, 8, sample_time/1000.);

float tmp = sample_time/1000.;

void setup() {
  // Change the frequency of the pwm.
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  // Set rst coefficient
  left_rst.set_rst(r1, s1, t1, order);
  right_rst.set_rst(r2, s2, t2, order);

  // Initialize serial connection.
  Serial.begin(115200);

  // Run the step_response
  // step_response(&left_motor, &left_encoder);
  // step_response(&right_motor, &right_encoder);

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
    // setpoint_position.x += 0.2;
    control_system();
  }
}


void control_system() {
  // Read motor position
  left_control = last_left_control;
  right_control = last_right_control;
  left_control.measurement = left_encoder.read();
  right_control.measurement = right_encoder.read();

  // Odometry
  odometry.update(left_control.measurement, right_control.measurement);

  // Update setpoint
  delta_move = setpoint.update();

  translation_ramp.compute(&delta_move->delta_translation);
  rotation_ramp.compute(&delta_move->delta_rotation);

  // Debug
  static uint8_t c = 50;
  if (c++ == 50) {
    // write_data(odometry.get_position(), sizeof(position_t));
    write_data(&setpoint_position, sizeof(position_t));
    // write_data(delta_move, sizeof(delta_move_t));
    c = 0;
  }

  // Update reference
  left_control.reference = left_control.measurement
    + cm2step(delta_move->delta_translation)*5 - rad2step(delta_move->delta_rotation)*5;
  right_control.reference = right_control.measurement
    + cm2step(delta_move->delta_translation)*5 + rad2step(delta_move->delta_rotation)*5;

  // Compute control command
  left_rst.compute();
  right_rst.compute();

  // Apply the command on the motors
  left_motor.set_pwm(left_control.command);
  right_motor.set_pwm(right_control.command);
}
