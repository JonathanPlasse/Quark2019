#include <Arduino.h>
#include <stdint.h>
#include <Encoder.h>
#include "motor.hpp"
#include "conversion.h"

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
#define M1_KP 0.034274
#define M1_KI 0.39104
#define M1_KD 0.
#define M2_KP 0.034467
#define M2_KI 0.35558
#define M2_KD 0.

Motor m1(M1_DIR1, M1_DIR2, M1_PWM, ENC1_PIN1, ENC1_PIN2, SAMPLE_TIME, M1_KP, M1_KI, M1_KD);
Motor m2(M2_DIR1, M2_DIR2, M2_PWM, ENC2_PIN1, ENC2_PIN2, SAMPLE_TIME, M2_KP, M2_KI, M2_KD);

uint32_t temps;

void setup() {
  /*Change the frequency of the pins 9, 10*/
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  Serial.begin(115200);
  m1.setSpeed(1633*1.7);
  m2.setSpeed(1633*1.7);
  temps = millis() - SAMPLE_TIME;
}


void loop() {
  if (millis() - temps > SAMPLE_TIME) {
    m1.run();
    m2.run();
    temps = millis();
  }
}
