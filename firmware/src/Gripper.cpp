#include "Gripper.h"

Gripper::Gripper(GripperPins pins) {
    this->pins = pins;
    pinMode(pins.dir, OUTPUT);
    pinMode(pins.enable, OUTPUT);

    release();
}

void Gripper::grip() {
    if (pins.inverted) {
        digitalWrite(pins.dir, LOW);
    } else {
        digitalWrite(pins.dir, HIGH);
    }
    digitalWrite(pins.enable, HIGH);
    #ifdef DEBUG
        Serial.println("Gripping");
    #endif
}

void Gripper::release() {
    if (pins.inverted) {
        digitalWrite(pins.dir, HIGH);
    } else {
        digitalWrite(pins.dir, LOW);
    }
    digitalWrite(pins.enable, HIGH);
    #ifdef DEBUG
        Serial.println("Releasing");
    #endif
}

