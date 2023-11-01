#pragma once

#include <ESP32SharpIR.h>

namespace robotathon::wall {

constexpr uint8_t PINS[3] = {12, 14, 27}; // orange, yellow, green
ESP32SharpIR irO{ESP32SharpIR::GP2Y0A21YK0F, PINS[0]};
ESP32SharpIR irY{ESP32SharpIR::GP2Y0A21YK0F, PINS[1]};
ESP32SharpIR irG{ESP32SharpIR::GP2Y0A21YK0F, PINS[2]};
// todo rename them not based on the wire color

void setup() {
    irO.setFilterRate(0.1f);
    irY.setFilterRate(0.1f);
    irG.setFilterRate(0.1f);
}

void loop() {
    float distO = 0.0;//irO.getDistanceFloat();
    float distY = 0.0;//irY.getDistanceFloat();
    float distG = irG.getDistanceFloat();
    Serial.printf("%.2f %.2f %.2f", distO, distY, distG);
}

}