#pragma once
#include <Arduino.h>

#include "Joint.h"
#include "Gripper.h"
#include "Joystick.h"
#include "RoboticArm.h"

// Debug mode
// #define DEBUG

// Interrupt time in ms
#define INTERRUPT_TIME 10

// Frequency used for the PWM
#define PWM_FREQUENCY 100.0

/*
// Joints Configuration
*/

// Pins used for the joints
#define basePins { 19, 22, 33, false }
#define shoulderPins { 18, 25, 32, false }
#define elbowPins { 17, 26, 35, false }
#define wristPins { 4, 27, 34, false }
#define gripperPins { 13, 12, true }

// ADC input <-> angle
#define baseAngleValues { {-90, 2987}, {90, 817} }
#define shoulderAngleValues { {0, 2270}, {76, 1310} }
#define elbowAngleValues { {0, 1750}, {-90, 2950} }
#define wristAngleValues { {-90, 1717}, {0, 640} }

// PID constants
#define basePID { 400, 10, 5 }
#define shoulderPID { 500, 400, 0 }
#define elbowPID { 500, 300, 0 }
#define wristPID { 100, 2, 0 }

// Arm dimensions
#define armDimensions { 110, 120, 160, 90 }

/*
// Joystick Configuration
*/

// Pins used for the joystick
#define joystickPins { 14, 15, 5 }

// Deadzone value
#define DEADZONE 2000

/*
// Board Configuration
*/

// Z value of the board. Used for grabbing and dropping pieces
#define BOARD_Z -60

// Height of the pieces. Used for calculating the Z value of the arm when moving to a square
#define PIECE_HEIGHT 60

// Free Z value. Calculated using the BOARD_Z and PIECE_HEIGHT
#define FREE_Z PIECE_HEIGHT + BOARD_Z

// Time to wait after a full movement, to guarantee the arm has stopped moving
#define MOVE_WAIT 200

// Time to wait after grabbing or releasing a piece, to guarantee the movement was completed
#define GRAB_RELEASE_WAIT 2000

// Step size used when moving the arm up and down. Used to ensure the arm moves linearly
#define UP_DOWN_STEP_SIZE 10

// Step size used when moving the arm left and right. Used to ensure the arm moves linearly
#define LEFT_RIGHT_STEP_SIZE 20

// X and Y values of the squares
#define SQUARE_POSITIONS { \
/* 1 */  { { 129,  130}, { 135,   90}, { 135,   56}, { 135,   20}, { 135,  -14}, { 135,  -50}, { 135,  -82}, { 135, -114}  }, \
/* 2 */  { { 163,  128}, { 171,   92}, { 172,   54}, { 174,   25}, { 174,  -11}, { 174,  -44}, { 168,  -60}, { 168, -109}  }, \
/* 3 */  { { 186,  124}, { 191,   97}, { 194,   66}, { 194,   30}, { 195,   -5}, { 194,  -39}, { 194,  -74}, { 189, -109}  }, \
/* 4 */  { { 215,  124}, { 222,   82}, { 219,   59}, { 219,   30}, { 221,  -15}, { 221,  -41}, { 216,  -73}, { 216, -105}  }, \
/* 5 */  { { 235,  117}, { 242,   90}, { 240,   63}, { 241,   30}, { 244,   -5}, { 243,  -35}, { 240,  -68}, { 238, -104}  }, \
/* 6 */  { {   0,    0}, { 255,   79}, { 261,   51}, { 261,   24}, { 261,  -11}, { 262,  -38}, { 258,  -70}, {   0,    0}  }, \
/* 7 */  { {   0,    0}, {   0,    0}, {   0,  0  }, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}  }, \
/* 8 */  { {   0,    0}, {   0,    0}, {   0,  0  }, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}  }  }
//              A             B             C             D             E             F             G             H

// Position to move the arm to when discarding a piece
#define DISCARD_PIECE_POSITION { 145, 199 }

// Position to move the arm to when idling
#define IDLE_POSITION { -4, 174 }