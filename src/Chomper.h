#include <Servo.h>
#include "Arduino.h"

#define CHOMP_INTERVAL 750 // milliseconds
#define START_CHOMP_ANGLE 120
#define END_CHOMP_ANGLE 82

struct Chomper {
  boolean m_chomping;
  Servo m_chomper;
  unsigned long m_lastChompTime;

  Chomper() = default;

  Chomper(int pin): m_chomping(false), m_lastChompTime(0) {
    m_chomper.attach(pin);
    m_chomper.write(START_CHOMP_ANGLE);
  }
    
   void chomp(unsigned long now) {
    if(!m_chomping)
      return;

    unsigned long delta = now - m_lastChompTime;

    if(delta > CHOMP_INTERVAL){
      m_lastChompTime = now;
      
      int angle = m_chomper.read();

      if(angle == START_CHOMP_ANGLE){
        Serial.println("chomp");
        m_chomper.write(END_CHOMP_ANGLE);
      }
      else if(angle == END_CHOMP_ANGLE){
        Serial.println("chomp2");
        m_chomper.write(START_CHOMP_ANGLE);
      }
      
    }
  }

};
