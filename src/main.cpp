#include "Configuration.h"
#include "Joystick.h"
#include "RoboticJoint.h"
#include "RoboticArm.h"

typedef struct {
    float x;
    float y;
    float z;
} Position;

typedef struct {
    float baseAngle;
    float shoulderAngle;
    float elbowAngle;
    float wristAngle;
    float gripperAngle;
} Angles;

Joystick joystick(joystickPins, DEADZONE);
Joystick secondJoystick(secondJoystickPins, DEADZONE);

RoboticJoint base(basePins, roboticJointPID);
RoboticJoint shoulder(shoulderPins, roboticJointPID);
RoboticJoint elbow(elbowPins, roboticJointPID);
RoboticJoint wrist(wristPins, roboticJointPID);
RoboticJoint gripper(gripperPins, roboticJointPID);

RoboticArm roboticArm(roboticArmDimensions, base, shoulder, elbow, wrist, gripper);

Position position = { 0, 0, 0 };
Angles angles = { 0, 0, 0, 0, 0 };

#if defined(ARDUINO_UNO)
ISR(TIMER1_COMPA_vect){
#elif defined(ARDUINO_MEGA)
ISR(TIMER4_COMPA_vect){
#endif
  roboticArm.control();
}

void setup() {
  Serial.begin(9600);
  position.z = 10;

  cli();
  #if defined(ARDUINO_UNO)
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = CLOCK_SPEED / (INTERRUPT_TIME * 8) - 1;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS21); // 8 prescaler
    TIMSK1 |= (1 << OCIE1A);
  #elif defined(ARDUINO_MEGA)
    TCCR4A = 0;
    TCCR4B = 0;
    TCNT4 = 0;
    OCR4A = CLOCK_SPEED / (INTERRUPT_TIME * 8) - 1;
    TCCR4B |= (1 << WGM12);
    TCCR4B |= (1 << CS21); // 8 prescaler
    TIMSK4 |= (1 << OCIE4A);
  #endif

  sei();
}

void controlAssisted() {
  JoystickValues joystickValues = joystick.getValues();
  
  position.x += joystickValues.x_axis / 1000.0;
  position.y += joystickValues.y_axis / 1000.0;

  roboticArm.moveTo(position.x, position.y, position.z);

  // Serial.print("X: ");
  // Serial.print(position.x);
  // Serial.print(" Y: ");
  // Serial.print(position.y);
  // Serial.print(" Z: ");
  // Serial.println(position.z);
}

void controlManual() {
  JoystickValues joystickValues = joystick.getValues();
  JoystickValues secondJoystickValues = secondJoystick.getValues();

  angles.baseAngle += joystickValues.x_axis / 1000.0;
  angles.shoulderAngle += joystickValues.y_axis / 1000.0;
  angles.elbowAngle += secondJoystickValues.x_axis / 1000.0;
  angles.wristAngle += secondJoystickValues.y_axis / 1000.0;

  angles.gripperAngle += joystickValues.button / 10.0;
  angles.gripperAngle -= secondJoystickValues.button / 10.0;

  if (angles.baseAngle > 90) angles.baseAngle = 90;
  if (angles.baseAngle < -90) angles.baseAngle = -90;
  if (angles.shoulderAngle > 90) angles.shoulderAngle = 90;
  if (angles.shoulderAngle < -90) angles.shoulderAngle = -90;
  if (angles.elbowAngle > 90) angles.elbowAngle = 90;
  if (angles.elbowAngle < -90) angles.elbowAngle = -90;
  if (angles.wristAngle > 90) angles.wristAngle = 90;
  if (angles.wristAngle < -90) angles.wristAngle = -90;
  if (angles.gripperAngle > 90) angles.gripperAngle = 90;
  if (angles.gripperAngle < -90) angles.gripperAngle = -90;

  Serial.print("Base: ");
  Serial.print(angles.baseAngle);
  Serial.print(" Shoulder: ");
  Serial.print(angles.shoulderAngle);
  Serial.print(" Elbow: ");
  Serial.print(angles.elbowAngle);
  Serial.print(" Wrist: ");
  Serial.print(angles.wristAngle);
  Serial.print(" Gripper: ");
  Serial.println(angles.gripperAngle);  
}

void loop() {
  controlManual();  
}