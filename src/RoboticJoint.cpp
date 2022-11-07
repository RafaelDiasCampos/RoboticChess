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

    int16_t pwm = pid.kp * error + pid.ki * errorSum + pid.kd * (error - lastError);
    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }
    int8_t dir = pwm > 0 ? HIGH : LOW;

    errorSum += error;
    lastError = error;

    digitalWrite(pins.dir, dir);
    analogWrite(pins.pwm, abs(pwm));
}
    