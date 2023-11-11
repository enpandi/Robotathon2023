#pragma once
#include "drive.h"
#include "led.h"

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
};

Color target{}, reading{};

void setup() {return;
    // Sets up I2C protocol
    wires.begin(SDA_PIN, SCL_PIN, FREQUENCY);

    // Set up color sensor
    apds.begin();
}

void read() {return;
    if (!apds.colorAvailable()) return;
    int r, g, b, a;
    apds.readColor(r, g, b, a);
    Serial.printf("rgba %d %d %d %d\n", r, g, b, a);
    reading = r > g && r > b ? Color::RED : g > r && g > b ? Color::GREEN : b > r && b > g ? Color::BLUE : Color::UNKNOWN;
}

enum State {
    SAMPLE, FIND, FOUND
} state;

void start() {return;
    target = Color::UNKNOWN;
    state = SAMPLE;
}

void loop() {return;
    read();
    if (target == Color::UNKNOWN) {
        if (reading != Color::UNKNOWN) {
            target = reading;
            Serial.printf("found target: %d\n", target);
        }
    } else {
        if (state == State::FIND && reading == Color::UNKNOWN) {
            state = State::FIND;
            Serial.printf("left sample: %d\n", target);
        } else if (state == State::FIND && target == reading) {
            state = State::FOUND;
            Serial.printf("found color: %d\n", reading);
        }
    }
    drive::l = drive::r = state == FOUND ? 0.0f : tuner::params.colorSpeed;
    int blinks = color::reading; // maxes out at 4 blinks
    switch (millis() % 2400 / 200) {
        case 4:
            if (blinks < 3) break;
        case 2:
            if (blinks < 2) break;
        case 0:
            if (blinks < 1) break;
            led::on = true;
            return;
        default:
            break;
    }
    led::on = false;
}

}