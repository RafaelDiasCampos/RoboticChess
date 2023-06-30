#pragma once
#include "Configuration.h"

typedef struct {
    float waist_length;
    float shoulder_length;
    float elbow_length;
    float wrist_length;
} RoboticArmDimensions;

class RoboticArm {
    public:
    RoboticArm(RoboticArmDimensions dimensions, Joint &base, Joint &shoulder, Joint &elbow, Joint &wrist, Gripper &gripper);
    void moveTo(float x, float y, float z);
    void setAngles(float baseAngle, float shoulderAngle, float elbowAngle, float wristAngle);
    void grab();
    void release();
    void control();

    private:
    RoboticArmDimensions dimensions;
    Joint &base;
    Joint &shoulder;
    Joint &elbow;
    Joint &wrist;
    Gripper &gripper;
};