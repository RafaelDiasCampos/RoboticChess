#pragma once
#include "Configuration.h"

typedef struct {
    int8_t x;
    int8_t y;
    int8_t button;
} JoystickPins;

typedef struct {
    int16_t x;
    int16_t y;
    bool button;
} JoystickValues;

class Joystick {
    public:
    Joystick(JoystickPins pins, int16_t deadzone);
    JoystickValues getValues();
    
    private:
    JoystickPins pins;
    int16_t deadzone;
};