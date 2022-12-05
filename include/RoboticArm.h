#pragma once
#include "Configuration.h"

typedef struct {
    float shoulder_length;
    float elbow_length;
    float wrist_length;
    float gripper_length;
} RoboticArmDimensions;

class RoboticArm {
    public:
    RoboticArm(RoboticArmDimensions dimensions, RoboticJoint& base, RoboticJoint& shoulder, RoboticJoint& elbow, RoboticJoint& wrist, RoboticJoint& gripper);
    void moveTo(float x, float y, float z);
    void setAngles(float baseAngle, float shoulderAngle, float elbowAngle, float wristAngle, float gripperAngle);
    void control();

    private:
    RoboticArmDimensions dimensions;
    RoboticJoint& base;
    RoboticJoint& shoulder;
    RoboticJoint& elbow;
    RoboticJoint& wrist;
    RoboticJoint& gripper;
};