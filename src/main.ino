#include <Arduino.h>
#include <stdint.h>
#include <Encoder.h>
#include "motor.hpp"
#include "conversion.h"

#define M_DIR1 A0
#define M_DIR2 A1
#define M_PWM 9
#define ENC_PIN1 2
#define ENC_PIN2 4
#define SAMPLE_TIME 100
#define KP 1
#define KI 0
#define KD 0

Motor m(M_DIR1, M_DIR2, M_PWM, ENC_PIN1, ENC_PIN2, SAMPLE_TIME, KP, KI, KD);

uint32_t temps;

void setup() {
  Serial.begin(115200);
  m.setSpeed(1633);
  temps = millis() - SAMPLE_TIME;
}


void loop() {
  if (millis() - temps > SAMPLE_TIME) {
    m.run();
    temps = millis();
  }
}
