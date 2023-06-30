#include <iostream>
#include <cmath>

#define PI 3.14159265
#define armDimensions { 110, 115, 155, 90 }

typedef struct {
    float waist_length;
    float shoulder_length;
    float elbow_length;
    float wrist_length;
} RoboticArmDimensions;

RoboticArmDimensions dimensions = armDimensions;

void moveTo(float x, float y, float z) {
    // Base angle is calculated by the angle between the origin and the point (x, y)
    float baseAngle = atan2(y, x) * 180 / PI;

    // Horizontal length required for the elbow + shoulder to reach the desired point
    float length = sqrt(x * x + y * y);
    // Vertical height required for the elbow + shoulder to reach the desired point
    float height = z - dimensions.waist_length + dimensions.wrist_length;

    // Law of cosines to calculate the angle between the shoulder and the elbow

    float elbowAngleRad = -acos((length * length + height * height - dimensions.shoulder_length * dimensions.shoulder_length - dimensions.elbow_length * dimensions.elbow_length ) / (2 * dimensions.shoulder_length * dimensions.elbow_length));
    float shoulderAngleRad = atan2(height, length) - atan2(dimensions.elbow_length * sin(elbowAngleRad), dimensions.shoulder_length + dimensions.elbow_length * cos(elbowAngleRad));

    // Convert radians to degrees
    float elbowAngle = elbowAngleRad * 180 / PI;
    float shoulderAngle = shoulderAngleRad * 180 / PI;

    // Wrist should be perpendicular to the ground
    float wristAngle = -90 - shoulderAngle - elbowAngle;

    std::cout << "Base angle: " << baseAngle << std::endl;
    std::cout << "Shoulder angle: " << shoulderAngle << std::endl;
    std::cout << "Elbow angle: " << elbowAngle << std::endl;
    std::cout << "Wrist angle: " << wristAngle << std::endl;
}

int main() {
    float x = 50;
    float y = 200;
    float z = 100;

    moveTo(x, y, z);
}