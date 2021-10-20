#include <Servo.h>
#include "Arduino.h"

struct Chomper {
  
  boolean chomping;
  Servo chomper;

  Chomper() = default;

   Chomper(int pin) {
    this->chomping = false;
    this->chomper.attach(pin);
    this->chomper.write(0);
  }
    
   boolean toggleChomp() {
    this->chomping = !this->chomping;
    if (this->chomping) {
      if (this->chomper.read() == 0){
        this->chomper.write(60);
      } else if (this->chomper.read() == 60) {
        this->chomper.write(0);
        Serial.print("Alex is cute");
      }
    }
    return this->chomping;
  }

};
