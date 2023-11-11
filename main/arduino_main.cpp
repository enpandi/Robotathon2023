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

#include "led.h"
#include "controller.h"
#include "tuner.h"
#include "drive.h"
#include "wall.h"
#include "line.h"
#include "color.h"
using namespace robotathon;

#include <Arduino.h>

// #include <Arduino.h>//
// extern const int adc_channel_io_map[SOC_ADC_PERIPH_NUM][SOC_ADC_MAX_CHANNEL_NUM];//

// Arduino setup function. Runs in CPU 1
void setup() {
    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    controller::setup();
    drive::setup();
    // wall::setup();
    line::setup();
    // color::setup();
    // led::setup();

    controller::dl.onUpDown = tuner::left;
    controller::dr.onUpDown = tuner::right;
    controller::du.onUpDown = tuner::up;
    controller::dd.onUpDown = tuner::down;
    controller::r.onUpDown = tuner::next;
    controller::l.onUpDown = tuner::prev;
    controller::zl.onUpDown = tuner::enable;
    controller::zl.onDownUp = tuner::disable;
    controller::a.onUpDown = line::calibrate;
    controller::x.onUpDown = color::resetReading;
    controller::x.onDown = color::read;
    controller::zr.onDown = line::loop;

    Serial.begin(115200);
    // Serial.printf("aaaaaaaaaaaaa %d %d\n", SOC_ADC_PERIPH_NUM, SOC_ADC_MAX_CHANNEL_NUM);
    // for (uint8_t i = 0; i < SOC_ADC_PERIPH_NUM; i++) {
    //     for (uint8_t j = 0; j < SOC_ADC_MAX_CHANNEL_NUM; j++) {
    //         Serial.printf("bbbbbbbbbbbb %d %d %d\n", i, j, adc_channel_io_map[i][j]);
    //     }
    // }
}

// Arduino loop function. Runs in CPU 1
void loop() {
    drive::l = drive::r = 0.0f;
    controller::loop();
    drive::loop();
    // wall::loop();
    // line::loop();
    // led::loop();
    vTaskDelay(1);
    // delay(100);
}
