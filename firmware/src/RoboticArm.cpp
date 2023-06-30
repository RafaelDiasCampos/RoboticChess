#include "RoboticArm.h"

RoboticArm::RoboticArm(RoboticArmDimensions dimensions, Joint &base, 
                      Joint &shoulder, Joint &elbow,
                      Joint &wrist, Gripper &gripper) : 
                      dimensions(dimensions), base(base), shoulder(shoulder), 
                      elbow(elbow), wrist(wrist), gripper(gripper) {

}

void RoboticArm::moveTo(float x, float y, float z) {
    // Base angle is calculated by the angle between the origin and the point (x, y)
    float baseAngle = atan2(y, x) * 180 / PI;

    // Horizontal length required for the elbow + shoulder to reach the desired point
    float length = sqrt(x * x + y * y);
    // Vertical height required for the elbow + shoulder to reach the desired point
    float height = z - dimensions.waist_length + dimensions.wrist_length;

    // Law of cosines to calculate the angle between the shoulder and the elbow
    float elbowAngleRad = -acos((length * length + height * height - dimensions.shoulder_length * dimensions.shoulder_length - dimensions.elbow_length * dimensions.elbow_length) / (2 * dimensions.shoulder_length * dimensions.elbow_length));
    float shoulderAngleRad = atan2(height, length) - atan2(dimensions.elbow_length * sin(elbowAngleRad), dimensions.shoulder_length + dimensions.elbow_length * cos(elbowAngleRad));

    // Convert radians to degrees
    float elbowAngle = elbowAngleRad * 180 / PI;
    float shoulderAngle = shoulderAngleRad * 180 / PI;

    // Wrist should be perpendicular to the ground
    float wristAngle = -90 - shoulderAngle - elbowAngle;

    setAngles(baseAngle, shoulderAngle, elbowAngle, wristAngle);

    #ifdef DEBUG
        // Serial.print("Base angle: ");
        // Serial.println(baseAngle);
        // Serial.print("Shoulder angle: ");
        // Serial.println(shoulderAngle);
        // Serial.print("Elbow angle: ");
        // Serial.println(elbowAngle);
        // Serial.print("Wrist angle: ");
        // Serial.println(wristAngle);
        Serial.print("Moving to X: ");
        Serial.print(x);
        Serial.print(", Y: ");
        Serial.print(y);
        Serial.print(", Z: ");
        Serial.println(z);
    #endif
}

void RoboticArm::setAngles(float baseAngle, float shoulderAngle, float elbowAngle, float wristAngle) {
    base.setAngle(baseAngle);
    shoulder.setAngle(shoulderAngle);
    elbow.setAngle(elbowAngle);
    wrist.setAngle(wristAngle);
}

void RoboticArm::grab() {
    gripper.grip();
}

void RoboticArm::release() {
    gripper.release();
}

void RoboticArm::control() {
    base.control();
    shoulder.control();
    elbow.control();
    wrist.control();

    // base.debug();
    // shoulder.debug();
    // elbow.debug();
    // wrist.debug();
}