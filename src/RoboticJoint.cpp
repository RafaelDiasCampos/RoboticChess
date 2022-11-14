#include "RoboticJoint.h"

RoboticJoint::RoboticJoint(RoboticJointPins pins, RoboticJointPID pid) : pins(pins), pid(pid) {
    pinMode(pins.dir, OUTPUT);
    pinMode(pins.pwm, OUTPUT);
    pinMode(pins.sensor, INPUT);
}

void RoboticJoint::setAngle(float angle) {
    this->angle = angle;
}

void RoboticJoint::control() {
    int16_t sensorValue = analogRead(pins.sensor);
    float sensorAngle = map(sensorValue, 0, 1023, -90, 90);

    float error = angle - sensorAngle;
    if (pins.inverted) {
        error = -error;
    }

    int16_t pwm;
    pwm  = pid.kp * error;
    pwm += pid.ki * errorSum * (INTERRUPT_TIME / 1000.0);
    pwm += pid.kd * (error - lastError) / (INTERRUPT_TIME / 1000.0);
    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }

    errorSum += error;
    lastError = error;

    #ifdef NO_INVERTER
        if (pwm > 0) {
            digitalWrite(pins.pwm, LOW);
            analogWrite(pins.dir, abs(pwm));
        }
        else {
            digitalWrite(pins.dir, LOW);
            analogWrite(pins.pwm, abs(pwm));
        }
    #else
        int8_t dir = pwm > 0 ? HIGH : LOW;
        digitalWrite(pins.dir, dir);
        analogWrite(pins.pwm, abs(pwm));
    #endif
}
    