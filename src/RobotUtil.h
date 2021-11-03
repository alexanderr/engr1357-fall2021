#ifndef ROBOT_UTIL_H
#define ROBOT_UTIL_H
#include <Arduino.h>
#include "RobotConstants.h"
#include <Servo.h>

namespace Ping {
    float get_ping(int trig, int echo) {
        digitalWrite(trig, LOW); //turn off the Trig pin incase it was on before
        delayMicroseconds(DELAY1); //a very short break
        
        digitalWrite(trig, HIGH); //turn on the Trig pin to send a sound wave
        delayMicroseconds(DELAY2); //a short break to let the operation happen
        digitalWrite(trig, LOW); //turn off the Trig pin to end the sound wave output
        
        float duration = pulseIn(echo, HIGH); //sensor the sound wave reflection time
        float distance  = (duration / 2) * SOUND_SPEED; // distance in centimeters.
        return distance;
    };
};

namespace KeypadConstants {
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

  char KEYS [KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };
  byte ROW_PINS[KEYPAD_ROWS] = {51, 49, 47, 45};
  byte COL_PINS[KEYPAD_COLS] = {43, 41, 39, 37};
};

#endif