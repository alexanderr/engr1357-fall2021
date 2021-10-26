#ifndef ENGR_PINGSENSOR_H
#define ENGR_PINGSENSOR_H



class PingSensor {
public:
  static const byte DELAY1 = 2; // microseconds
  static const byte DELAY2 = 10; // microseconds
  static constexpr float SOUND_SPEED = 0.0343; // 343 m/s in 0.0343 cm/us

  byte m_trig;
  byte m_echo;

  PingSensor(): m_trig(0), m_echo(0) {}

  PingSensor(byte trig, byte echo): m_trig(trig), m_echo(echo) {
      pinMode(m_echo,INPUT);
      pinMode(m_trig,OUTPUT);
  }

  float getDistance() {
    digitalWrite(m_trig, LOW); //turn off the Trig pin incase it was on before
    delayMicroseconds(DELAY1); //a very short break
    
    digitalWrite(m_trig, HIGH); //turn on the Trig pin to send a sound wave
    delayMicroseconds(DELAY2); //a short break to let the operation happen
    digitalWrite(m_trig, LOW); //turn off the Trig pin to end the sound wave output
    
    float duration = pulseIn(m_echo, HIGH); //sensor the sound wave reflection time
    float distance  = (duration / 2) * SOUND_SPEED; // distance in centimeters.
    return distance;
  }

};

#endif
