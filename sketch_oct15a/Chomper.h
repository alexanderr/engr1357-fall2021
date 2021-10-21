#include <Servo.h>
#include "Arduino.h"

#define CHOMP_INTERVAL 1500 // milliseconds

struct Chomper {
  boolean m_chomping;
  Servo m_chomper;
  unsigned long m_lastChompTime;

  Chomper() = default;

  Chomper(int pin): m_chomping(false), m_lastChompTime(0) {
    m_chomper.attach(pin);
    m_chomper.write(0);
  }
    
   void chomp(unsigned long now) {
    if(!m_chomping)
      return;

    unsigned long delta = now - m_lastChompTime;

    if(delta > CHOMP_INTERVAL){
      m_lastChompTime = now;
      
      int angle = m_chomper.read();

      if(angle == 0){
        m_chomper.write(60);
      }
      else if(angle == 60){
        m_chomper.write(0);
      }
      
    }
  }

};
