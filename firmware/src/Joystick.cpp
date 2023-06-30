#include "Joystick.h"

Joystick::Joystick(JoystickPins pins, int16_t deadzone) : pins(pins), deadzone(deadzone) {
    pinMode(pins.x, INPUT);
    pinMode(pins.y, INPUT);
    pinMode(pins.button, INPUT);
}

JoystickValues Joystick::getValues() {
    JoystickValues values;
    values.x = analogRead(pins.x) - 2048;
    values.y = analogRead(pins.y) - 2048;
    values.button = !digitalRead(pins.button);

    if (abs(values.x) < deadzone) {
        values.x = 0;
    }
    if (abs(values.y) < deadzone) {
        values.y = 0;
    }

    return values;
}