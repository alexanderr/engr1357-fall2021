#include "Robot.h"
#include "Path.h"

Robot::Robot() {
    m_lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
    m_lcd->begin(16, 2); // 16x2 lcd
    m_lcd->backlight();
    m_keypad = new Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
    m_pingL = PingSensor(Pins::PING1_TRIG, Pins::PING1_ECHO);
    m_pingR = PingSensor(Pins::PING2_TRIG, Pins::PING2_ECHO);
    m_controller = MovementFSM(Pins::M_FRONTLEFT, Pins::M_FRONTRIGHT, Pins::M_BACKLEFT, Pins::M_BACKRIGHT);
    m_chomper = Chomper(Pins::SERVO);

    m_irX = IRSensor(Pins::IR_X);
    m_irY = IRSensor(Pins::IR_Y);

    pinMode(Pins::INCLINOMETER, INPUT);

    state = RobotState::STATIONARY;
    m_path = nullptr;
};


void Robot::loop()  {
    checkKeypad();
    ping();
    m_irX.scan();
    m_irY.scan();

    unsigned long now = millis();

    // m_chomper.chomp(now);

    if(m_distanceL < COLLISION_THRESHOLD){
      state |= RobotState::LEFT_COLL;
    }
    else {
      state &= ~RobotState::LEFT_COLL;
    }

    if(m_distanceR < COLLISION_THRESHOLD){
      state |= RobotState::RIGHT_COLL;
    }
    else {
      state &= ~RobotState::RIGHT_COLL;
    }

    if(isTurningLeft()){
      state |= RobotState::TURNING_LEFT;
    } else {
      state &= ~RobotState::TURNING_LEFT;
    }

    if(isTurningRight()){
      state |= RobotState::TURNING_RIGHT;
    }
    else {
      state &= ~RobotState::TURNING_RIGHT;
    }

    if(m_controller.currentState == &ForwardState::getInstance()){
      state |= RobotState::FORWARD;
    }
    else {
      state &= ~RobotState::FORWARD;
    }

//    if(isReversing()){
//      state |= RobotState::REVERSING;
//    }
//    else {
//      state &= ~RobotState::REVERSING;
//    }

    if(m_path)
      m_path->loop(now);

    if(m_irX.onlyContainsChar(X_CHAR_GOAL)){
      state |= RobotState::ALIGNED_X;
    }
    else {
      state &= ~RobotState::ALIGNED_X;
    }

    if(m_irY.onlyContainsChar(Y_CHAR_GOAL)){
      state |= RobotState::ALIGNED_Y;
    }
    else {
      state &= ~RobotState::ALIGNED_Y;
    }

    displayState();
    delay(100);
}
