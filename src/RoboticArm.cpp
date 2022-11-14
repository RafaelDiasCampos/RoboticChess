#include "RoboticArm.h"

RoboticArm::RoboticArm(RoboticArmDimensions dimensions, RoboticJoint& base, 
                      RoboticJoint& shoulder, RoboticJoint& elbow,
                      RoboticJoint& wrist, RoboticJoint& gripper) : 
                      dimensions(dimensions), base(base), shoulder(shoulder), 
                      elbow(elbow), wrist(wrist), gripper(gripper) {

}

void RoboticArm::moveTo(float x, float y, float z) {
    float baseAngle = atan2(y, x) * 180 / PI;
    float length = sqrt(x * x + y * y);

    float shoulderAngle = acos((length * length + z * z - dimensions.shoulder_length * dimensions.shoulder_length - dimensions.elbow_length * dimensions.elbow_length) / (2 * dimensions.shoulder_length * dimensions.elbow_length)) * 180 / PI;
    float elbowAngle = acos((dimensions.shoulder_length * dimensions.shoulder_length + dimensions.elbow_length * dimensions.elbow_length - length * length - z * z) / (2 * dimensions.shoulder_length * dimensions.elbow_length)) * 180 / PI;

    float wristAngle = -90 + shoulderAngle + elbowAngle;
    float gripperAngle = 0;

    base.setAngle(baseAngle);
    shoulder.setAngle(shoulderAngle);
    elbow.setAngle(elbowAngle);
    wrist.setAngle(wristAngle);
    gripper.setAngle(gripperAngle);
}

void RoboticArm::control() {
    base.control();
    shoulder.control();
    elbow.control();
    wrist.control();
    gripper.control();
}