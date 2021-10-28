#include <Servo.h>
#include "Arduino.h"

struct SalinityArm {
    Servo arm;
    byte pin;
    bool enabled = false;

    SalinityArm(int pin);   
    void start() {arm.attach(pin); arm.write(90); };
    void stop() { arm.detach(); arm.write(0); }; 
    void toggle() {
        enabled = !enabled;
        if(enabled) start();
        else stop();
    }
};
