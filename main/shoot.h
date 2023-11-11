#pragma once
#include "controller.h"
#include "tuner.h"

#include <ESP32Servo.h>

namespace robotathon::shoot {

constexpr int PIN = 4;
constexpr int MIN = 1000;
constexpr int MAX = 2000;
Servo servo;
int dir;

void setup() {
    servoL.setPeriodHertz(50);
    servoR.attach(PIN, MIN, MAX);
}

int toPulseWidth(float x) {
    return (MIN * (1.0f - x) + MAX * (1.0f + x)) / 2.0f;
}

void loop() {    // Serial.printf("%.2f %.2f\n", l, r);
    servo.write(toPulseWidth(dir * tuner::shooterSpeed * tuner::params.servoLMultiplier + tuner::params.servoLOffset));
}

}