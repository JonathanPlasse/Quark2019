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
#define SAMPLE_TIME 10
#define KP 0.5
#define KI 0
#define KD 0

//Test Moteur
#define MEASURE_TIME 500
#define WAIT_TIME 500
#define PWM 220

Motor m1(M1_DIR1, M1_DIR2, M1_PWM, ENC1_PIN1, ENC1_PIN2, SAMPLE_TIME, KP, KI, KD);
Motor m2(M2_DIR1, M2_DIR2, M2_PWM, ENC2_PIN1, ENC2_PIN2, SAMPLE_TIME, KP, KI, KD);

uint32_t lastTime, time, startTime;
float speed;
uint16_t sampleTime, nbMeasure, nbMeasureDone;

void setup() {
  /*Change the frequency of the pins 9, 10*/
  TCCR1B = (TCCR1B & 0xf8) | 0x01;

  Serial.begin(115200);

  //Ask sample time
  sampleTime = Serial.parseInt();
  m1.setSampleTime(sampleTime);

  //Ask the number of measure to do
  nbMeasure = Serial.parseInt();
  nbMeasureDone = 0;

  startTime = lastTime = millis() - SAMPLE_TIME;
  m1.setPwm(PWM);
}


void loop() {
  time = millis();
  if (nbMeasureDone < nbMeasure && time - lastTime > SAMPLE_TIME) {
    lastTime += SAMPLE_TIME;
    m1.computeSpeed();

    if (time - startTime < MEASURE_TIME) {
      m1.computeSpeed();
      speed = m1.getActualSpeed();
      Serial.print(speed);
      Serial.print(" ");
    }
    else if (time - startTime < MEASURE_TIME + WAIT_TIME) {
      m1.stop();
    }
    else {
      startTime = lastTime;
      nbMeasureDone++;
      Serial.println("");
      m1.setPwm(PWM);
    }
  }
}
