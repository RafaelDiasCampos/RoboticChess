#pragma once
#include "Configuration.h"

typedef struct {
    int8_t dir;
    int8_t pwm;
    int8_t sensor;
    bool inverted;
} RoboticJointPins;

typedef struct {
    float kp;
    float ki;
    float kd;
} RoboticJointPID;

class RoboticJoint {
    public:
    RoboticJoint(RoboticJointPins pins, RoboticJointPID pid);
    void setAngle(float angle);
    void control();

    private:
    RoboticJointPins pins;
    float angle;

    RoboticJointPID pid;
    float errorSum;
    float lastError; 
};