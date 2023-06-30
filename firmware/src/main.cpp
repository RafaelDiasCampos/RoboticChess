#include "Configuration.h"
#include "Chess.h"

hw_timer_t *timer;
volatile bool interruptFlag = false;
int interruptCount = 0;

Joint base = Joint(basePins, basePID, baseAngleValues);
Joint shoulder = Joint(shoulderPins, shoulderPID, shoulderAngleValues);
Joint elbow = Joint(elbowPins, elbowPID, elbowAngleValues);
Joint wrist = Joint(wristPins, wristPID, wristAngleValues);
Gripper gripper = Gripper(gripperPins);

RoboticArm arm = RoboticArm(armDimensions, base, shoulder, elbow, wrist, gripper);

Joystick joystick = Joystick(joystickPins, DEADZONE);

SquarePosition squarePositions[8][8] = SQUARE_POSITIONS;
Chess chess = Chess(arm, squarePositions, DISCARD_PIECE_POSITION, IDLE_POSITION);

int x = 163, y = -21;

void IRAM_ATTR onTimer(){
  interruptFlag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  arm.moveTo(x, y, FREE_Z);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, INTERRUPT_TIME * 1000, true);
  timerAlarmEnable(timer);
}

void testJoystick() {
  auto values = joystick.getValues();
  bool changedPosition = false;
  if (values.y > 0) {
    y += 1;
    changedPosition = true;
  } else if (values.y < 0) {
    y -= 1;
    changedPosition = true;
  }

  if (values.x > 0) {
    x += 1;
    changedPosition = true;
  } else if (values.x < 0) {
    x -= 1;
    changedPosition = true;
  }

  if (changedPosition) {
    arm.moveTo(x, y, BOARD_Z);
  }
}

void parseCommand(String &command) {
  command.trim();
  command.toLowerCase();
  if (command.startsWith("grab")) {
    chess.grab();
  } else if (command.startsWith("release")) {
    chess.release();
  } else if (command.startsWith("move")) {
    char column = command.charAt(5);
    int y = column - 'a';
    int x = command.substring(6).toInt() - 1;
    chess.moveToSquare(x, y);
  } else if (command.startsWith("discard")) {
    chess.discard();
  } else if (command.startsWith("reset")) {
    chess.reset();
  } else if (command.startsWith("wait")) {
    int time = command.substring(5).toInt();
    chess.wait(time);
  }
}

void readJoystick() {
  auto values = joystick.getValues();
  if (values.x > 0) {
    Serial.println("joystick right");
  } else if (values.x < 0) {
    Serial.println("joystick left");
  } else if (values.y > 0) {
    Serial.println("joystick up");
  } else if (values.y < 0) {
    Serial.println("joystick down");
  } else if (values.button) {
    Serial.println("select");
  }
}

void loop() {
  if (interruptFlag) {
    arm.control();
    if (interruptCount % 10 == 0) {
      chess.handle();
      if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        parseCommand(command);
      }
      if (interruptCount % 100 == 0) {
        readJoystick();
      }
      // testJoystick();
    }
    interruptCount++;
    interruptFlag = false;
  }
}