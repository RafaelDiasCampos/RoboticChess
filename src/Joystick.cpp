#include "Joystick.h"

Joystick::Joystick(JoystickPins pins, int16_t deadzone) : pins(pins), deadzone(deadzone) {
    pinMode(pins.x_axis, INPUT);
    pinMode(pins.y_axis, INPUT);
    pinMode(pins.button, INPUT);
}

JoystickValues Joystick::getValues() {
    JoystickValues values;
    values.x_axis = analogRead(pins.x_axis) - 512;
    values.y_axis = analogRead(pins.y_axis) - 512;
    values.button = digitalRead(pins.button);

    if (abs(values.x_axis) < deadzone) {
        values.x_axis = 0;
    }
    if (abs(values.y_axis) < deadzone) {
        values.y_axis = 0;
    }

    return values;
}