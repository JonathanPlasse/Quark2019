#include <Arduino.h>
#include <stdint.h>
#include <Encoder.h>
#include "motor.hpp"
#include "binary_serial.hpp"

//Moteur Gauche
#define M1_DIR1 A0
#define M1_DIR2 A1
#define M1_PWM 9
#define ENC1_PIN1 2
#define ENC1_PIN2 4

//Moteur Droit
#define M2_DIR1 A2
#define M2_DIR2 A3
#define M2_PWM 10
#define ENC2_PIN1 3
#define ENC2_PIN2 5

//Asserevissement
#define SAMPLE_TIME 5
#define KP 0
#define KI 0
#define KD 0

typedef struct {
  uint8_t nb_measure;
  uint16_t nb_sample;
  uint16_t wait_time;
  uint8_t pwm;
} configStruct;

typedef struct {
  uint32_t timestamp;
  uint32_t position;
  float speed;
} measureStruct;

configStruct config;
measureStruct measure;

Motor m1(M1_DIR1, M1_DIR2, M1_PWM, ENC1_PIN1, ENC1_PIN2,
         SAMPLE_TIME, KP, KI, KD);
Motor m2(M2_DIR1, M2_DIR2, M2_PWM, ENC2_PIN1, ENC2_PIN2,
         SAMPLE_TIME, KP, KI, KD);

uint32_t last_time, last_wait_time;
uint16_t nb_measure_done, nb_sample_done;


void step_response() {
  if (nb_sample_done < config.nb_sample) {
    measure.position = m1.get_position();
    measure.speed = m1.get_actual_speed();
    write_data(&measure, sizeof(measure));
    nb_sample_done++;
  }
  else if (nb_sample_done < config.nb_sample + config.wait_time / SAMPLE_TIME) {
    m1.stop();
    nb_sample_done++;
  }
  else if (nb_measure_done < config.nb_measure - 1) {
    ++nb_measure_done;
    nb_sample_done = 0;
    m1.set_pwm(config.pwm);
  }
}


void setup() {
  /*Change the frequency of the pins 9, 10*/
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  Serial.begin(115200);
  read_data(&config, sizeof(config));
  write_data(&config, sizeof(config));

  nb_measure_done = 0;
  nb_sample_done = 0;

  last_time = millis();

  m1.set_pwm(config.pwm);
}

void loop() {
  measure.timestamp = millis();

  if (measure.timestamp - last_time > SAMPLE_TIME) {
    last_time += SAMPLE_TIME;
    m1.compute_speed();

    step_response();
  }
}
