#include "main.hpp"
#include <Arduino.h>
#include <stdint.h>
#include "binary_serial.hpp"

// Left Motor
#define M1_DIR1 A0
#define M1_DIR2 A1
#define M1_PWM 9
#define ENC1_PIN1 2
#define ENC1_PIN2 4

// Right Motor
#define M2_DIR1 A2
#define M2_DIR2 A3
#define M2_PWM 10
#define ENC2_PIN1 3
#define ENC2_PIN2 5

typedef struct {
  uint8_t pwm;
  uint8_t sample_time;
  uint8_t nb_measure;
  uint16_t nb_sample;
  uint16_t wait_time;
} configStruct;

typedef struct {
  uint32_t timestamp;
  uint32_t position;
  float speed;
} measureStruct;

configStruct config;
measureStruct measure;

Motor motor1(M1_DIR1, M1_DIR2, M1_PWM);
Motor motor2(M2_DIR1, M2_DIR2, M2_PWM);

Encoder encoder1(ENC1_PIN1, ENC1_PIN2);
Encoder encoder2(ENC2_PIN1, ENC2_PIN2);

const uint8_t order = 2;
float r1[order+1] = {17.66, -31.48, 14.01};
float s1[order+1] = {1., -0.7265 -0.2735};
float t1[order+1] = {2.466, -2.275, 0};
float r2[order+1] = {19.94, -35.67, 15.94};
float s2[order+1] = {1., -0.7262, -0.2738};
float t2[order+1] = {2.784, -2.587, 0};

float reference1 = 1633*10, measurement1, control1;
float reference2 = 1633*10, measurement2, control2;
float min_control = -255, max_control = 255;

Rst rst1(&reference1, &measurement1, &control1, min_control, max_control);
Rst rst2(&reference2, &measurement2, &control2, min_control, max_control);

uint32_t sample_time = 10;
uint32_t time, last_time, last_position;
uint16_t nb_measure_done, nb_sample_done;


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
  timer(millis(), sample_time);
}


void timer(uint32_t time, uint32_t sample_time) {
  static uint32_t last_time = millis();

  if (time - last_time > sample_time) {
    // Run the step_response
    // step_response(&motor1, &encoder1);
    // step_response(&motor2, &encoder2);
    control_system();
  }
}


void control_system() {
  // Update last_time
  last_time += sample_time;

  measurement1 = encoder1.read();
  measurement2 = encoder2.read();

  rst1.compute();
  rst2.compute();

  motor1.set_pwm(control1);
  motor2.set_pwm(control2);
}


bool step_response(Motor* motor, Encoder* encoder) {
  // Indicate if initialization has been done
  static bool init = false;

  // Initialize
  if (!init) {
    // Receive configuration
    read_data(&config, sizeof(config));

    // Set sample time
    sample_time = config.sample_time;

    // Initialize counter
    nb_measure_done = 0;
    nb_sample_done = 0;

    // Power motor
    motor->set_pwm(config.pwm);

    init = true;
  }

  if (nb_measure_done < config.nb_measure) {
    measure.timestamp = millis();
    last_position = measure.position;
    measure.position = encoder->read();
    measure.speed = (float)(measure.position-last_position)
      / config.sample_time * 1000;

    if (nb_sample_done < config.nb_sample) {
      write_data(&measure, sizeof(measure));
      nb_sample_done++;
    }
    else if (nb_sample_done < config.nb_sample + config.wait_time / config.sample_time) {
      motor->set_pwm(0);
      nb_sample_done++;
    }
    else if (nb_measure_done + 1 < config.nb_measure) {
      nb_measure_done++;
      nb_sample_done = 0;
      motor->set_pwm(config.pwm);
    }
    return false;
  }
  // Step response done
  init = false;
  return true;
}
