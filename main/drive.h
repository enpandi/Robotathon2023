#pragma once
#include "controller.h"
#include "tuner.h"

#include <ESP32Servo.h>

namespace robotathon::drive {

constexpr int PIN_L = 15;
constexpr int PIN_R = 13;
constexpr int MIN = 1000;
constexpr int MAX = 2000;
Servo servoL, servoR;
float l{}, r{};

void setup() {
    servoL.setPeriodHertz(50);
    servoL.attach(PIN_L, MIN, MAX);
    servoR.setPeriodHertz(50);
    servoR.attach(PIN_R, MIN, MAX);
}

int toPulseWidth(float x) {
    return (MIN * (1.0f - x) + MAX * (1.0f + x)) / 2.0f;
}

void loop() {
    // Serial.printf("%.2f %.2f\n", l, r);
    servoL.write(toPulseWidth(l * tuner::params.servoLMultiplier + tuner::params.servoLOffset));
    servoR.write(toPulseWidth(r * tuner::params.servoRMultiplier + tuner::params.servoROffset));
}

}