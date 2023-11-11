#pragma once
#include <ArduinoConsole.h>

namespace robotathon::tuner {

class Parameter {
    const char* name;
    float value, min, max, tick;
public:
    Parameter(const char* name, float value, float min, float max, float tick)
        : name{name}
        , value{value}
        , min{min}
        , max{max}
        , tick{tick} {}
    operator float() const { return value; }
    void left() { tick *= 10.0f; }
    void right() { tick *= 0.1f; }
    void up() { value += tick; if (value > max) value = max; }
    void down() { value -= tick; if (value < min) value = min; }
    void print() {
        Console.printf("%s: [%f] +%f\n",
            name,
            value,
            tick
        );
    }
};

struct {
    Parameter lineKp{"line Kp",   0.05f, 0.0f, 0.14f, 0.001f};
    Parameter lineKi{"line Ki", 0.0008f, 0.0f, 0.001f, 0.00001f};
    Parameter lineKd{"line Kd",    0.7f, 0.0f, 1.2f, 0.01f};
    Parameter servoLOffset{"servoL offset", 0.067f, -1.0f, +1.0f, 0.01f};
    Parameter servoROffset{"servoR offset", -0.236f, -1.0f, +1.0f, 0.01f};
    Parameter servoLMultiplier{"servoL multiplier", 0.568109f, +0.01f, +1.0f, 0.01f};
    Parameter servoRMultiplier{"servoR multiplier", 0.666f, +0.01f, +1.0f, 0.01f};
} params;
const int NUM_PARAMS = sizeof(params) / sizeof(Parameter);
int index{};
bool tuning{};

#define VALUES reinterpret_cast<Parameter*>(&params)
void print() { VALUES[index].print(); }
void left() { if (tuning) { VALUES[index].left(); print(); } }
void right() { if (tuning) { VALUES[index].right(); print(); } }
void up() { if (tuning) { VALUES[index].up(); print(); } }
void down() { if (tuning) { VALUES[index].down(); print(); } }
#undef VALUES

void prev() { if (tuning) { --index; index += NUM_PARAMS; index %= NUM_PARAMS; } }
void next() { if (tuning) { ++index; index %= NUM_PARAMS; } }
void enable() { tuning = true; print(); }
void disable() { tuning = false; print(); }

}