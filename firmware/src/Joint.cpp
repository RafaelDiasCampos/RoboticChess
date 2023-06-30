#include "Joint.h"

int8_t Joint::pwmChannels = 0;

Joint::Joint(JointPins pins, JointPID pid, JointAngleValues angleValues) : pins(pins), pid(pid), angleValues(angleValues) {
    pinMode(pins.dir, OUTPUT);
    pinMode(pins.pwm, OUTPUT);
    pinMode(pins.sensor, INPUT);
    
    ledcSetup(pwmChannels, PWM_FREQUENCY, 16);
    ledcAttachPin(pins.pwm, pwmChannels);
    this->pins.pwm = pwmChannels++;
}

void Joint::setAngle(float angle) {
    int16_t value = map(angle, angleValues.calibrationFirst.angle, angleValues.calibrationSecond.angle, angleValues.calibrationFirst.analogValue, angleValues.calibrationSecond.analogValue);
    this->desiredValue = value;
}

void Joint::control() {
    int16_t sensorValue = analogRead(pins.sensor);

    int16_t error = desiredValue - sensorValue;
    if (pins.inverted) {
        error = -error;
    }

    float pwm;
    pwm  = pid.kp * error;
    pwm += pid.ki * errorSum * (INTERRUPT_TIME / 1000.0);
    pwm += pid.kd * (error - lastError) / (INTERRUPT_TIME / 1000.0);
    pwm = constrain(pwm, (-2<<16) -1, (2<<16) -1);

    errorSum += error;
    lastError = error;
    int8_t dir = pwm > 0 ? HIGH : LOW;
    digitalWrite(pins.dir, dir);
    ledcWrite(pins.pwm, abs(pwm));
}

void Joint::debug() {
    Serial.print("Desired value: ");
    Serial.print(desiredValue);
    Serial.print(" Sensor value: ");
    Serial.print(analogRead(pins.sensor));
    Serial.print(" Error: ");
    Serial.print(desiredValue - analogRead(pins.sensor));
    Serial.print(" PWM: ");
    Serial.println(ledcRead(pins.pwm));
}