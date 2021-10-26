#include "Servo.h"
#include "SalinityArm.h"


SalinityArm::SalinityArm(int pin) {
    arm.attach(pin);
}

void SalinityArm::moveDown() {
    arm.write(DOWN_ANGLE);
}

void SalinityArm::moveUp() {
    arm.write(UP_ANGLE);
}
