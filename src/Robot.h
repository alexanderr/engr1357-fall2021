#ifndef ROBOT_H
#define ROBOT_H
#include <Keypad.h>
#include <LCD.h> //I2C library
#include <LiquidCrystal_I2C.h>
#include "MovementFSM.h"
#include "PingSensor.h"
#include "Chomper.h"
#include "IRSensor.h"
#include "RobotConstants.h"
#include "Actions.h"
#include "SalinityArm.h"


/* constants */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define DEFAULT_COLLISION_THRESHOLD 30.0

#define X_CHAR_GOAL 'K'
#define Y_CHAR_GOAL 'G'
#define IR_BUFFER_SIZE 8

struct ActionManager;

struct Robot: public MovementFSM {
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
  
  float m_distanceL;
  float m_distanceR;
  float m_collisionThreshold;

  unsigned int state;

  IRSensor m_irX;
  IRSensor m_irY;
  ActionManager* m_actionMgr;
  SalinityArm* m_salinityArm;

  Robot();

  void checkKeypad();

  bool inline isTurning() { return state & RobotState::TURNING_MASK; }
  bool inline isReversing() { return state & RobotState::REVERSING_MASK; }
  bool inline isMoving() { return state & RobotState::MOVING_MASK; }
  bool inline hasFrontCollision() { return state & RobotState::COLLISION_MASK; }
  bool inline hasLeftCollision() { return state & RobotState::LEFT_COLL; }
  bool inline hasRightCollision() { return state & RobotState::RIGHT_COLL; }

  int getInclinometerReading() {
    return analogRead(Pins::INCLINOMETER); // 0 - 1023
  }

  int getSalinityReading() {
    return analogRead(Pins::SALINITY_SENSOR); // 0 - 1023
  }

  void ping();

  void displayState(){
    m_lcd->clear();
    m_lcd->setCursor(0, 0);
    m_lcd->print(String(state));
  }

  
  void loop();

  
  
};

#endif
