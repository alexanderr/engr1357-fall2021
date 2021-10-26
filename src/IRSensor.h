#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <Arduino.h>

// IR DETAILS
#define EVENT_RISING 1
#define EVENT_FALLING 2
#define EVENT_NONE 0
#define IR_WAIT 100000

#define IR_BUFFER_SIZE 8

class IRSensor {
public:
  // Instance variables used in conjunction with the IR sensor
  unsigned char necState;
  int num_chars;
  unsigned long prev_time;
  
  unsigned char IRChar, IRCharBitMask;
  unsigned char buffer[IR_BUFFER_SIZE];
  bool receiverState;
  unsigned long cur_time, ticks;
  byte m_pin;

  IRSensor() = default;

  IRSensor(byte pin): m_pin(pin), receiverState(false) {
    memset(buffer, 0, IR_BUFFER_SIZE);
  }

  int scan() {
      // takes 13 ms per char to broadcast from a beacon
      char reading;
      unsigned char event;
      // reset the buffer
      memset(buffer, 0, IR_BUFFER_SIZE);
      num_chars = 0;

      unsigned long IRCounter = 0;

      // Cycle while we try to watch for a character.
      while (IRCounter < IR_WAIT)
      {
          // Digital level from IR receiver will be inverted
          if (digitalRead(m_pin))
          {
              event = receiverState ? EVENT_FALLING : EVENT_NONE;
              receiverState = false;
          }
          else
          {
              event = !receiverState ? EVENT_RISING : EVENT_NONE;
              receiverState = true;
          }
          
          if (event != EVENT_NONE)
          {
              cur_time = micros();
              ticks = cur_time - prev_time;
              if (necState == 0)
              { // Expecting rising edge of leading pulse
                  if (event == EVENT_RISING)
                  {
                      necState = 1;
                  }
              }
              else if (necState == 1)
              { // Expecting falling edge of leading pulse
                  if (event == EVENT_FALLING)
                  {
                      if (ticks > 8900L)
                      {
                          necState = 2; // Check for leading pulse > 8.9msec
                      }
                      else
                      { // Stray short pulse found, reset NEC state
                          necState = 0;
                      }
                  }
              }
              else if (necState == 2)
              { // Expecting rising edge of first pulse after leading pulse
                  if (event == EVENT_RISING)
                  {
                      if (ticks > 3375L)
                      { // Check for space after leading pulse > 3.375 msec
                          IRCharBitMask = 0x80;
                          IRChar = 0;
                          necState = 3;
                      }
                      else
                      { // Space too short, reset NEC state to wait for another leading pulse
                          necState = 0;
                      }
                  }
              }
              else if (necState == 3)
              { // Expecting falling edge of data pulse
                  if (event == EVENT_FALLING)
                  {
                      if (ticks < 648)
                      {
                          necState = 4; // Check if data pulse width < 648 usec
                      }
                      else
                      { // Width too short, reset NEC state to wait for another leading pulse
                          necState = 0;
                      }
                  }
              }
              else if (necState == 4)
              { // Expecting rising edge of pulse after data pulse
                  if (event == EVENT_RISING)
                  {
                      if (ticks > 1120)
                      { // Record a '1' bit for space > 1120 usec
                          IRChar = IRChar | IRCharBitMask;
                      }
                      IRCharBitMask = IRCharBitMask >> 1;

                      if (IRCharBitMask == 0)
                      {                               // Check if eighth bit received and character complete (!!!!!)
                          buffer[num_chars] = IRChar; // Record complete character received in circular output buffer
                          num_chars++;
                          necState = 0; // Reset NEC state to wait for another leading pulse
                      }
                      else
                      {
                          necState = 3; // Wait for falling edge of data pulse
                      }
                  }
              }
              prev_time = cur_time;
          }
          IRCounter++;
      }
      return num_chars;
  }

  bool hasChar(char expected) {
    for(int i = 0; i < IR_BUFFER_SIZE; ++i){
      if(buffer[i] == expected)
        return true;
    }

    return false;
  }

  bool onlyContainsChar(char expected){
    for(int i = 0; i < IR_BUFFER_SIZE; ++i){
      if(buffer[i] != expected)
        return false;
    }

    return true;
  }
};

#endif
