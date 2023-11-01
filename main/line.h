#pragma once

#include <QTRSensors.h>

namespace robotathon::line {

constexpr uint8_t NUM_SENSORS{8};
constexpr uint8_t PINS[NUM_SENSORS] = {26, 25, 33, 32, 35, 34, 39, 36};
QTRSensors qtr;

void calibrate() {
    static constexpr int CALIBRATION_ITERATIONS = 100;
    qtr.resetCalibration();
    Serial.print("calibrating reflectance sensor ");
    for (uint8_t i = 0; i < CALIBRATION_ITERATIONS; i++) {
        if (!(i % 10))
            Serial.print('.');
        qtr.calibrate(QTRReadMode::OddEvenAndOff);
        delay(1);
    }
    Serial.println(" done");
}

void setup() {
    qtr.setTypeAnalog();
    qtr.setSensorPins(PINS, 8);
}

void loop() {
    uint16_t sensors[8];
    // qtr.readCalibrated(sensors);
    int16_t position = qtr.readLineBlack(sensors) - (NUM_SENSORS - 1) * 500;
    Serial.printf("[%4u %4u %4u %4u %4u %4u %4u %4u] %4d %s\n",
        sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5], sensors[6], sensors[7],
        position, position < 0 ? "On the left" : position > 0 ? "On the right" : "Straight Ahead");
}

}