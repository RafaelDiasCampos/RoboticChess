#pragma once
#include "Configuration.h"

typedef struct {
    int8_t x_axis;
    int8_t y_axis;
    int8_t button;
} JoystickPins;

typedef struct {
    int16_t x_axis;
    int16_t y_axis;
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