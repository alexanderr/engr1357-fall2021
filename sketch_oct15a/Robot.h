#ifndef ROBOT_H
#define ROBOT_H
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "MovementFSM.h"
#include "PingSensor.h"
#include "Chomper.h"
#include "IRSensor.h"

class Path;


/* constants */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define COLLISION_THRESHOLD 30.0

#define X_CHAR_GOAL 'K'
#define Y_CHAR_GOAL 'G'
#define IR_BUFFER_SIZE 8

namespace Pins {
  enum {    
    M_FRONTRIGHT = 9,
    M_BACKRIGHT = 5,

    M_FRONTLEFT = 10,
    M_BACKLEFT = 6,
    
    SERVO = 11,

    PING1_ECHO = 29,
    PING1_TRIG = 27,

    PING2_TRIG = 31,
    PING2_ECHO = 33,

    INCLINOMETER = 7,

    IR_X = 1,
    IR_Y = 2,
  };
};


struct Robot {
  char KEYPAD_KEYS [KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };
  byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {51, 49, 47, 45};
  byte KEYPAD_COL_PINS[KEYPAD_COLS] = {43, 41, 39, 37};
  Keypad* m_keypad;
  LiquidCrystal_I2C* m_lcd;

  Chomper m_chomper;
  PingSensor m_pingL;
  PingSensor m_pingR;
  
  MovementFSM m_controller;
  float m_distanceL;
  float m_distanceR;
  unsigned int state;

  IRSensor m_irX;
  IRSensor m_irY;
  Path* m_path;

  Robot();

  bool isTurning() {
    return (m_controller.currentState == &TurnLeftState::getInstance()) || (m_controller.currentState == &TurnRightState::getInstance());
  }

  bool isTurningLeft() {
    m_controller.currentState == &TurnLeftState::getInstance();
  }

  bool isTurningRight() {
    m_controller.currentState == &TurnRightState::getInstance();
  }

  bool isMoving() {
    return m_controller.currentState != &StationaryState::getInstance();
  }

  bool isReversing() {
    return m_controller.currentState == &ReverseTurnLeftState::getInstance();
  }

  void checkKeypad() {
    char key = m_keypad->getKey();
    switch(key){
      case '1': m_controller.setState(ForwardState::getInstance()); break;
      case '*': m_controller.setState(StationaryState::getInstance()); break;
      case 'A': m_chomper.m_chomping = !m_chomper.m_chomping; break;
    }
  }

  int getInclinometerReading() {
    return analogRead(Pins::INCLINOMETER); // 0 - 1023
  }

  void ping() {
    m_distanceL = m_pingL.getDistance();
    m_distanceR = m_pingR.getDistance();
  }

  void displayState(){
    m_lcd->clear();
    m_lcd->setCursor(0, 0);

    if(isTurningLeft()){
      m_lcd->print("TurnLeft");
    }
    else if(isTurningRight()){
      m_lcd->print("TurnRight");
    }
    else if(isReversing()){
      m_lcd->print("Reversing");
    }
    else if(isMoving()){
      m_lcd->print("Forward");
    }
    else {
      m_lcd->print("Stationary");
    }

    m_lcd->setCursor(0, 1);
    m_lcd->print("collecting:" + String(m_chomper.m_chomping));
  }

  void oldMovement() {
    static unsigned long lastWallTime = 0;
    
    bool leftCollision = m_distanceL < COLLISION_THRESHOLD;
    bool rightCollision = m_distanceR < COLLISION_THRESHOLD;
    
    if(isMoving()){
      if(leftCollision && rightCollision){
        m_controller.setState(ReverseTurnLeftState::getInstance());
        lastWallTime = millis();
      }
      else if(leftCollision){
        m_controller.setState(TurnRightState::getInstance());
      }
      else if(rightCollision){
        m_controller.setState(TurnLeftState::getInstance());
      }
      else if(isTurning() || isReversing()){ // no collision
        m_controller.setState(ForwardState::getInstance());
      }
    }
  }

  void loop();

  
  
};

#endif
