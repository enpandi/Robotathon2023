#pragma once
#include "color.h"

namespace robotathon::led {

constexpr uint8_t PIN = 2;

void setup() {
    pinMode(PIN, OUTPUT);
}

void loop() {
    int blinks = color::reading; // maxes out at 4 blinks
    switch (millis() % 2400 / 200) {
        case 4:
            if (blinks < 3) break;
        case 2:
            if (blinks < 2) break;
        case 0:
            if (blinks < 1) break;
            digitalWrite(PIN, 1);
            return;
        default:
            break;
    }
    digitalWrite(PIN, 0);
    // digitalWrite(PIN, (millis() / 1000 & 1) && controller::gamepads[0]);
}

}