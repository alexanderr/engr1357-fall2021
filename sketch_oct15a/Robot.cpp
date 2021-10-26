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

  state = 0;
  m_path = nullptr;
};

void Robot::ping(){
  m_distanceL = m_pingL.getDistance();
  m_distanceR = m_pingR.getDistance();
}


void Robot::checkKeypad()  {
  char key = m_keypad->getKey();
  switch(key){
    case '1': m_controller.setState(ForwardState::getInstance()); break;
    case '*': m_controller.setState(StationaryState::getInstance()); break;
    case 'A': m_chomper.m_chomping = !m_chomper.m_chomping; break;
  }
}


void Robot::loop()  {
  checkKeypad();
  ping();
  m_irX.scan();
  m_irY.scan();

  unsigned long now = millis();

  // Reset the state while we turn on the bits.
  state = 0;

  // Set the movement bits.
  if (m_controller.currentState == &ForwardState::getInstance()) state |= RobotState::FORWARD;
  else if(m_controller.currentState == &TurnLeftState::getInstance()) state |= RobotState::TURNING_LEFT;
  else if(m_controller.currentState == &TurnRightState::getInstance()) state |= RobotState::TURNING_RIGHT;
  else if(m_controller.currentState == &ReverseTurnLeftState::getInstance()) state |= RobotState::REVERSING_LEFT;
  else if(m_controller.currentState == &ReverseTurnRightState::getInstance()) state |= RobotState::REVERSING_RIGHT;

  // Set the collision bits.
  if(m_distanceL < COLLISION_THRESHOLD) state |= RobotState::LEFT_COLL;
  if(m_distanceR < COLLISION_THRESHOLD) state |= RobotState::RIGHT_COLL;

  // Set the IR alignment bits.
  if(m_irX.onlyContainsChar(X_CHAR_GOAL)) state |= RobotState::ALIGNED_X;
  if(m_irY.onlyContainsChar(Y_CHAR_GOAL)) state |= RobotState::ALIGNED_Y;

    
  if(m_path)
    m_path->loop(now);

  displayState();
  delay(100);
}
