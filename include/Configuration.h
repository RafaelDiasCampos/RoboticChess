#pragma once
#include <Arduino.h>

#include "Joystick.h"
#include "RoboticJoint.h"

#define INTERRUPT_TIME 10
#define DEADZONE 100
// #define NO_INVERTER

#if defined(ARDUINO_UNO)
    #define CLOCK_SPEED 16000000

    #define joystickPins { A0, A1, 0 }

    #define basePins { 1, 7, A2 }
    #define shoulderPins { 2, 8, A2 }
    #define elbowPins { 3, 9, A2 }
    #define wristPins { 4, 10, A2 }
    #define gripperPins { 5, 11, A2 }

#elif defined(ARDUINO_MEGA)
    #define CLOCK_SPEED 16000000

    #define joystickPins { A15, A14, 28 }
    #define secondJoystickPins { A13, A12, 29 }

    #define basePins { 22, 13, A11 }
    #define shoulderPins { 23, 12, A10 }
    #define elbowPins { 24, 11, A9 }
    #define wristPins { 25, 10, A8 }
    #define gripperPins { 26, 9, A7 }

#endif

#define roboticJointPID { 50, 1, 0.01 }
#define roboticArmDimensions { 10, 10, 10, 10 }