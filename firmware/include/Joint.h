#pragma once

class Joint;

#include "Configuration.h"

typedef struct {
    int8_t dir;
    int8_t pwm;
    int8_t sensor;
    bool inverted;
} JointPins;

typedef struct {
    float kp;
    float ki;
    float kd;
} JointPID;

typedef struct {
    int16_t angle;
    int16_t analogValue;
} JointCalibrationPoint;

typedef struct {
    JointCalibrationPoint calibrationFirst;
    JointCalibrationPoint calibrationSecond;
} JointAngleValues;

class Joint {
    public:
    Joint(JointPins pins, JointPID pid, JointAngleValues angleValues);
    void setAngle(float angle);
    void control();
    void debug();

    private:
    static int8_t pwmChannels;
    JointPins pins;
    JointPID pid;
    JointAngleValues angleValues;

    int16_t desiredValue;
    
    int errorSum = 0;
    int lastError = 0; 
};