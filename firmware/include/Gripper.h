#pragma once

class Gripper;

#include "Configuration.h"

typedef struct {
    int8_t dir;
    int8_t enable;
    bool inverted;
} GripperPins;

class Gripper {
    public:
    Gripper(GripperPins pins);
    void grip();
    void release();

    private:
    GripperPins pins;
};