#pragma once

#include <Wire.h>
#include <Arduino_APDS9960.h>

namespace robotathon::color {

// https://www.arduino.cc/reference/en/libraries/arduino_apds9960/readcolor/

constexpr int INTERRUPT_PIN = 2;
constexpr int SDA_PIN = 22;
constexpr int SCL_PIN = 23;
constexpr uint32_t FREQUENCY = 100000;

TwoWire wires{0};
APDS9960 apds{wires, INTERRUPT_PIN};

enum Color {
    UNKNOWN=0, RED=1, GREEN=2, BLUE=3
} reading{};

void setup() {
    // Sets up I2C protocol
    wires.begin(SDA_PIN, SCL_PIN, FREQUENCY);

    // Set up color sensor
    apds.begin();
}

void resetReading() {
    reading = Color::UNKNOWN;
}

void read() {
    if (!apds.colorAvailable()) return;
    int r, g, b, a;
    apds.readColor(r, g, b, a);
    reading = r > g && r > b ? Color::RED : g > r && g > b ? Color::GREEN : b > r && b > g ? Color::BLUE : Color::UNKNOWN;
}

}