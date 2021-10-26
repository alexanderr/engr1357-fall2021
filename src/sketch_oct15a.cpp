#include <Arduino.h>
#include "Robot.h"


Robot* robot;

void setup() {
  robot = new Robot();
  Serial.begin(9600);
}

void loop() {
  robot->loop();
}
