#pragma once
#include "esp32-hal-gpio.h"

namespace robotathon::led {

constexpr uint8_t PIN = 2;
bool on{};

void setup() {
    pinMode(PIN, OUTPUT);
}

void loop() {
    digitalWrite(PIN, on);
}

}