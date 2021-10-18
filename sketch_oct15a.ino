#include "Robot.h"

Robot robot;

void setup() {
  Serial.begin(9600);
}

void loop() {
  robot.loop();
  delay(50);
}
