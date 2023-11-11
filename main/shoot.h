#pragma once
#include "controller.h"
#include "tuner.h"

#include <ESP32Servo.h>

namespace robotathon::shoot {

constexpr int PIN = 33;
constexpr int MIN = 1000;
constexpr int MAX = 2000;
Servo servo;
int dir;

void setup() {
    servo.setPeriodHertz(50);
    servo.attach(PIN, MIN, MAX);
}

int toPulseWidth(float x) {
    return (MIN * (1.0f - x) + MAX * (1.0f + x)) / 2.0f;
}

void loop() {    // Serial.printf("%.2f %.2f\n", l, r);
    Serial.printf("shoot %d\n", toPulseWidth(controller::ly * tuner::params.shooterSpeed * tuner::params.servoSMultiplier + tuner::params.servoSOffset));
    servo.write(toPulseWidth(controller::ly * tuner::params.shooterSpeed * tuner::params.servoSMultiplier + tuner::params.servoSOffset));
}

}