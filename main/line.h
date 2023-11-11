#pragma once

#include <QTRSensors.h>

namespace robotathon::line
{

    constexpr uint8_t NUM_SENSORS{8};
    constexpr uint8_t PINS[NUM_SENSORS] = {26, 25, 33, 32, 35, 34, 39, 36};

    // float Kp = 0.05;
    // float Ki = 0.00001;
    // float Kd = 0.6;

    int P;
    int I;
    int D;
    int lastError = 0;

    QTRSensors qtr;

    void calibrate()
    {
        static constexpr int CALIBRATION_ITERATIONS = 100;
        qtr.resetCalibration();
        Serial.print("calibrating reflectance sensor ");
        for (uint8_t i = 0; i < CALIBRATION_ITERATIONS; i++)
        {
            if (!(i % 10))
                Serial.print('.');
            qtr.calibrate(QTRReadMode::OddEvenAndOff);
            delay(1);
        }
        Serial.println(" done");
    }

    void setup()
    {
        qtr.setTypeAnalog();
        qtr.setSensorPins(PINS, 8);
    }

    void forward_movement(float speedA, float speedB)
    {
        drive::l = speedA;
        drive::r = speedB;
        Serial.printf("%.2f %.2f\n", drive::l, drive::r);
    }

    void PID_control()
    {
        uint16_t sensors[NUM_SENSORS];
        uint16_t positionLine = qtr.readLineBlack(sensors);

        int error = 3500 - positionLine;

        P = error;
        I = error + I;
        D = error - lastError;
        lastError = error;

        int motorSpeedChange = P * tuner::params.lineKp + I * tuner::params.lineKi + D * tuner::params.lineKd;

        float motorSpeedA = 0.3 + (drive::l - motorSpeedChange)  / 500;
        float motorSpeedB = 0.3 + (drive::r + motorSpeedChange) / 500;

        if (motorSpeedA > 0.75)
            motorSpeedA = 0.75;
        if (motorSpeedA < -0.75)
            motorSpeedA = -0.75;
        if (motorSpeedB > 0.75)
            motorSpeedB = 0.75;
        if (motorSpeedB < -0.75)
            motorSpeedB = -0.75;

        forward_movement(motorSpeedA, motorSpeedB);

        Serial.printf("[%4u %4u %4u %4u %4u %4u %4u %4u]\tmsa,msb,pos,err = %.2f %.2f %d %d\n\n",
            sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5], sensors[6], sensors[7],
            motorSpeedA, motorSpeedB, positionLine, error);
    }

    void loop()
    {
        uint16_t sensors[NUM_SENSORS];
        // qtr.readCalibrated(sensors);
        float suggestion = qtr.readLineBlack(sensors) / ((NUM_SENSORS - 1) * 500.0f) - 1.0f;
        Serial.printf("line suggestion %.2f; %04d %04d %04d %04d %04d %04d %04d %04d\n",
            suggestion,
            sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5], sensors[6], sensors[7]
        );
        if (-tuner::params.lineThreshold < suggestion && suggestion < tuner::params.lineThreshold) {
            drive::l = drive::r = 1.0f;
        } else {
            drive::l = drive::r = tuner::params.turnSpeed;
            if (suggestion < 0) drive::l = -drive::l;
            else drive::r = -drive::r;
        }
        
        // PID_control();
    }

}