#include <Arduino.h>
#include <stdint.h>
#include <Encoder.h>
#include "motor.hpp"
#include "conversion.h"

#define M_DIR1 A0
#define M_DIR2 A1
#define M_PWM 9
#define SAMPLE_TIME 10
#define KP 0
#define KI 0
#define KD 0

Encoder enc(2, 4);
Motor m(M_DIR1, M_DIR2, M_PWM, enc, SAMPLE_TIME, KP, KI, KD);

uint32_t temps;

void setup() {
  m.setSpeed(cmToPas(10));
  temps = millis() - SAMPLE_TIME;
}


void loop() {
  if (millis() - temps > SAMPLE_TIME) {
    m.run();
    temps = millis();
  }
}
