#include <Servo.h>
#include "Arduino.h"

struct SalinityArm {
    enum {
        UP_ANGLE = 0,
        DOWN_ANGLE = 90,
    };

    Servo arm;

    SalinityArm(int pin);   
    void moveUp();
    void moveDown(); 
};
