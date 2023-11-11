#pragma once

#include <ESP32SharpIR.h>

namespace robotathon::wall {

constexpr uint8_t PIN_L = 12;
constexpr uint8_t PIN_R = 14;
ESP32SharpIR irL{ESP32SharpIR::GP2Y0A21YK0F, PIN_L};
// ESP32SharpIR irY{ESP32SharpIR::GP2Y0A21YK0F, PINS[1]};
ESP32SharpIR irR{ESP32SharpIR::GP2Y0A21YK0F, PIN_R};
// todo rename them not based on the wire color

void setup() {
    irL.setFilterRate(0.1f);
    irR.setFilterRate(0.1f);
}

void loop() {
    float l = irL.getDistanceFloat();
    float r = irR.getDistanceFloat();
    Serial.printf("l r %.2f %.2f\n", l, r);
    float suggestion = r - l;
    if (-tuner::params.wallThreshold < suggestion && suggestion < tuner::params.wallThreshold) {
        drive::l = drive::r = 1.0f;
    } else {
        drive::l = drive::r = tuner::params.turnSpeed;
        if (suggestion < 0) drive::l = -drive::l;
        else drive::r = -drive::r;
    }
}

}