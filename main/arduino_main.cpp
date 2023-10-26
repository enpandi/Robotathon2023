/****************************************************************************
Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

#include "controller.h"
#include "tuner.h"

//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

GamepadPtr myGamepads[BP32_MAX_GAMEPADS]{};
Robotathon::Controller ctrl;
bool tuning;
Robotathon::Tuner tuner;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (!myGamepads[i]) {
            Console.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            GamepadProperties properties = gp->getProperties();
            Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName(), properties.vendor_id,
                           properties.product_id);
            myGamepads[i] = gp;
            return;
        }
    }
    Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
}

void onDisconnectedGamepad(GamepadPtr gp) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            return;
        }
    }
    Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
}



Servo servoL, servoR;
ESP32SharpIR sensor1( ESP32SharpIR::GP2Y0A21YK0F, 27);
QTRSensors qtr;
constexpr uint8_t LED_PIN = 2;

// Arduino setup function. Runs in CPU 1
void setup() {
    // Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    // BP32.forgetBluetoothKeys();

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
    servoL.setPeriodHertz(50);
    servoL.attach(15, 1000, 2000);
    servoR.setPeriodHertz(50);
    servoR.attach(13, 1000, 2000);

    Serial.begin(115200);
    // sensor1.setFilterRate(0.1f);

    // qtr.setTypeRC(); // or setTypeAnalog()
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]) {26, 25, 33, 32, 35, 34, 39, 36}, 8);
    for (uint8_t i = 0; i < 250; i++) {
        Serial.println("calibrating");
        qtr.calibrate();
        delay(20);
    }
    // qtr.calibrate();

    pinMode(LED_PIN, OUTPUT);

#if 0
    struct S {
        static void du() { Console.println("du"); }
        static void dd() { Console.println("dd"); }
        static void dl() { Console.println("dl"); }
        static void dr() { Console.println("dr"); }
        static void a() { Console.println("a"); }
        static void b() { Console.println("b"); }
        static void x() { Console.println("x"); }
        static void y() { Console.println("y"); }
        static void l() { Console.println("l"); }
        static void r() { Console.println("r"); }
        static void zl() { Console.println("zl"); }
        static void zr() { Console.println("zr"); }
        static void sl() { Console.println("sl"); }
        static void sr() { Console.println("sr"); }
        static void m() { Console.println("m"); }
        static void p() { Console.println("p"); }
        static void h() { Console.println("h"); }
    };
    ctrl.du.onDown = S::du;
    ctrl.dd.onDown = S::dd;
    ctrl.dl.onDown = S::dl;
    ctrl.dr.onDown = S::dr;
    ctrl.a.onDown = S::a;
    ctrl.b.onDown = S::b;
    ctrl.x.onDown = S::x;
    ctrl.y.onDown = S::y;
    ctrl.l.onDown = S::l;
    ctrl.r.onDown = S::r;
    ctrl.zl.onDown = S::zl;
    ctrl.zr.onDown = S::zr;
    ctrl.sl.onDown = S::sl;
    ctrl.sr.onDown = S::sr;
    ctrl.m.onDown = S::m;
    ctrl.p.onDown = S::p;
    ctrl.h.onDown = S::h;
#endif
    struct F {
        static void enableTuning() { tuning = true; tuner.print(); }
        static void disableTuning() { tuning = false; }
        static void tuningNext() { if (tuning) { tuner.next(); tuner.print(); } }
        static void tuningPrev() { if (tuning) { tuner.prev(); tuner.print(); } }
        static void tuningLeft() { if (tuning) { tuner.left(); tuner.print(); } }
        static void tuningRight() { if (tuning) { tuner.right(); tuner.print(); } }
        static void tuningUp() { if (tuning) { tuner.up(); tuner.print(); } }
        static void tuningDown() { if (tuning) { tuner.down(); tuner.print(); } }
    };
    ctrl.zl.onUpDown = F::enableTuning;
    ctrl.zl.onDownUp = F::disableTuning;
    ctrl.r.onUpDown = F::tuningNext;
    ctrl.l.onUpDown = F::tuningPrev;
    ctrl.dl.onUpDown = F::tuningLeft;
    ctrl.dr.onUpDown = F::tuningRight;
    ctrl.du.onUpDown = F::tuningUp;
    ctrl.dd.onUpDown = F::tuningDown;
}

// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected()) {
            ctrl.read(myGamepad);
            float l = ctrl.ly * tuner.params.servoLMultiplier + tuner.params.servoLOffset;
            float r = ctrl.ry * tuner.params.servoRMultiplier + tuner.params.servoROffset;
            servoL.write(l * 500 + 1500);
            servoR.write(r * 500 + 1500);
#if 0
            servoL.write( ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );
            servoR.write( ((((float) myGamepad->axisRY()) / 512.0f) * 500) + 1500 );

            // Another way to query the buttons, is by calling buttons(), or
            // miscButtons() which return a bitmask.
            // Some gamepads also have DPAD, axis and more.
            // Console.printf("servo.write %.3f\n", ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );
            // Console.printf("servo.write %.3f\n", ((((float) myGamepad->axisRY()) / 512.0f) * 500) + 1500 );
            Console.printf(
                "idx=%d dpad=0x%02x buttons=0x%04x axisX=%4d axisY=%4d axisRX=%4d axisRY=%4d misc=0x%02x\n",
                i,                        // Gamepad Index
                myGamepad->dpad(),        // DPAD
                myGamepad->buttons(),     // bitmask of pressed buttons
                myGamepad->axisX(),       // (-511 - 512) left X Axis
                myGamepad->axisY(),       // (-511 - 512) left Y axis
                myGamepad->axisRX(),      // (-511 - 512) right X axis
                myGamepad->axisRY(),      // (-511 - 512) right Y axis
                // myGamepad->brake(),       // (0 - 1023): brake button
                // myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
                myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
            );

            // You can query the axis and other properties as well. See Gamepad.h
            // For all the available functions.
#endif
        }
    }

    // Serial.println(sensor1.getDistanceFloat());

    uint16_t sensors[8];
    int16_t position = qtr.readLineBlack(sensors);
    Serial.printf("[%4d %4d %4d %4d %4d %4d %4d %4d] %4d %s\n",
        sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5], sensors[6], sensors[7],
        position, position < 0 ? "On the left" : position > 0 ? "On the right" : "Straight Ahead");
    vTaskDelay(1);
    // delay(100);

    digitalWrite(LED_PIN, (millis() / 1000 & 1) && myGamepads[0]);
}
