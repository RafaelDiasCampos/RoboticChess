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

void loop() {
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