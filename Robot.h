#ifndef ROBOT_H
#define ROBOT_H
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "MovementFSM.h"
#include "PingSensor.h"

/* constants */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

namespace Pins {
  enum {
    PING2_TRIG = 2,
    PING2_ECHO = 4,

    
    M_FRONTRIGHT = 9,
    M_BACKRIGHT = 5,

    M_FRONTLEFT = 10,
    M_BACKLEFT = 6,
    
    SERVO = 11,

    PING1_ECHO = 29,
    PING1_TRIG = 27,

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
  Keypad* InputKeypad;
  LiquidCrystal_I2C* lcd;
  
  PingSensor Ping1;
  PingSensor Ping2;
  
  MovementFSM controller;

  Robot() {
    lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
    lcd->begin(16, 2); // 16x2 lcd
    lcd->backlight();
    InputKeypad = new Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
    Ping1 = PingSensor(Pins::PING1_TRIG, Pins::PING1_ECHO);
    Ping2 = PingSensor(Pins::PING2_TRIG, Pins::PING2_ECHO);
    controller = MovementFSM(Pins::M_FRONTLEFT, Pins::M_FRONTRIGHT, Pins::M_BACKLEFT, Pins::M_BACKRIGHT);
  };

  bool isTurning() {
    return (controller.currentState == &TurnLeftState::getInstance()) || (controller.currentState == &TurnRightState::getInstance());
  }

  bool isMoving() {
    return controller.currentState != &StationaryState::getInstance();
  }

  void checkKeypad() {
    char key = InputKeypad->getKey();
    switch(key){
      case '1': controller.setState(ForwardState::getInstance()); break;
      case '*': controller.setState(StationaryState::getInstance()); break;
    }
  }

  void loop() {
    checkKeypad();
    float distance = Ping1.getDistance();
    if((distance < 30.0) && !isTurning() { //obstacle
        controller.setState(TurnLeftState::getInstance());
    }
    else if(isTurning()){
      controller.setState(ForwardState::getInstance());
    }
  }
  
};





#endif
