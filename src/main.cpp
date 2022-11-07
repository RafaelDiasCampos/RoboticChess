#include "Configuration.h"
#include "Joystick.h"
#include "RoboticJoint.h"
#include "RoboticArm.h"

typedef struct {
    float x;
    float y;
    float z;
} Position;

Joystick joystick(joystickPins, DEADZONE);

RoboticJoint base(basePins, roboticJointPID);
RoboticJoint shoulder(shoulderPins, roboticJointPID);
RoboticJoint elbow(elbowPins, roboticJointPID);
RoboticJoint wrist(wristPins, roboticJointPID);
RoboticJoint gripper(gripperPins, roboticJointPID);

RoboticArm roboticArm(roboticArmDimensions, base, shoulder, elbow, wrist, gripper);

Position position = { 0, 0, 0 };

void setup() {
  Serial.begin(9600);
  position.z = 10;
}

void loop() {
  JoystickValues joystickValues = joystick.getValues();
  
  position.x += joystickValues.x_axis / 1000.0;
  position.y += joystickValues.y_axis / 1000.0;

  roboticArm.moveTo(position.x, position.y, position.z);

  Serial.print("X: ");
  Serial.print(position.x);
  Serial.print(" Y: ");
  Serial.print(position.y);
  Serial.print(" Z: ");
  Serial.println(position.z);
}