#pragma once
#include <Arduino.h>

#include "Joystick.h"
#include "RoboticJoint.h"


#define DEADZONE 100

#if defined(ARDUINO_UNO)

    #define joystickPins { A0, A1, 0 }

    #define basePins { 1, 7, A2 }
    #define shoulderPins { 2, 8, A2 }
    #define elbowPins { 3, 9, A2 }
    #define wristPins { 4, 10, A2 }
    #define gripperPins { 5, 11, A2 }

#elif defined(ARDUINO_MEGA)

    #define joystickPins { A15, A14, 30 }

    #define basePins { 32, 13, A13 }
    #define shoulderPins { 33, 12, A12 }
    #define elbowPins { 34, 11, A11 }
    #define wristPins { 35, 10, A10 }
    #define gripperPins { 36, 9, A9 }

#endif

#define roboticJointPID { 5, 0.001, 0.01 }
#define roboticArmDimensions { 10, 10, 10, 10 }