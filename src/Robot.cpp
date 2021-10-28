#include "Robot.h"

#define ROBOT_TICKRATE 50 // milliseconds


Robot::Robot(): MovementFSM(Pins::M_FRONTLEFT, Pins::M_FRONTRIGHT, Pins::M_BACKLEFT, Pins::M_BACKRIGHT) {
  m_lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
  m_lcd->begin(16, 2); // 16x2 lcd
  m_lcd->backlight();
  m_keypad = new Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROWS, KEYPAD_COLS);
  m_pingF = PingSensor(Pins::PINGF_TRIG, Pins::PINGF_ECHO);
  m_pingL = PingSensor(Pins::PINGL_TRIG, Pins::PINGL_ECHO);
  m_pingR = PingSensor(Pins::PINGR_TRIG, Pins::PINGR_ECHO);
  m_chomper = Chomper(Pins::COLLECTOR_SERVO);

  m_distanceF = m_distanceL = m_distanceR = 0.0;

  m_irX = IRSensor(Pins::IR_X);
  m_irY = IRSensor(Pins::IR_Y);

  pinMode(Pins::INCLINOMETER, INPUT);
  m_collisionThreshold = DEFAULT_COLLISION_THRESHOLD;
  state = 0;
  m_actionMgr = new ActionManager;
  m_salinityArm = new SalinityArm(Pins::SALINITY_ARM);
};


#define PING_BUFFER 1

float Robot::getMedianDistance(PingSensor& sensor) {
  return sensor.getDistance();
  // static float buffer[PING_BUFFER];
  // for(int i = 0; i < PING_BUFFER; ++i){
  //   buffer[i] =  sensor.getDistance();
  // }
  // RobotUtil::insertionSort(buffer, PING_BUFFER);
  // return buffer[PING_BUFFER / 2];
}

void Robot::ping(){
  m_distanceF = getMedianDistance(m_pingF);
  m_distanceL = getMedianDistance(m_pingL);
  m_distanceR = getMedianDistance(m_pingR);
}


void Robot::checkKeypad()  {
  char key = m_keypad->getKey();
  switch(key){
    case '1': setMovementState(ForwardState::getInstance()); break;
    case '*': setMovementState(StationaryState::getInstance()); break;
    case 'A': m_chomper.m_chomping = !m_chomper.m_chomping; break;
    case '2': m_actionMgr->setActionList(ActionManager::MAZELEFT, NUM_MAZEACTIONS); break;
    case '3': m_actionMgr->setActionList(ActionManager::SALINITYRIGHT, NUM_SALINITYACTIONS); break;
    case '4': m_actionMgr->setActionList(ActionManager::UNIT_TEST, NUM_TESTACTIONS); break;
    case '5': m_actionMgr->setActionList(ActionManager::TAKE_SALINE, 1); break;
    // case 'E': m_actionMgr->setActionList(ActionManager::SALINITYRIGHT); break;
  }
}


void Robot::loop()  {
  static unsigned int i = 0;

  checkKeypad();

  switch(i % 4) {
    case 0: m_distanceF = getMedianDistance(m_pingF); break;
    case 1: m_distanceL = getMedianDistance(m_pingL); break;
    case 2: m_distanceR = getMedianDistance(m_pingR); break;
  }

  // m_irX.scan();
  // m_irY.scan();

  unsigned long now = millis();

  // Reset the state while we turn on the bits.
  state = 0;

  // Set the movement bits.
  if (movementState == &ForwardState::getInstance()) state |= RobotState::FORWARD;
  else if(movementState == &TurnLeftState::getInstance()) state |= RobotState::TURNING_LEFT;
  else if(movementState == &TurnRightState::getInstance()) state |= RobotState::TURNING_RIGHT;
  else if(movementState == &ReverseTurnLeftState::getInstance()) state |= RobotState::REVERSING_LEFT;
  else if(movementState == &ReverseTurnRightState::getInstance()) state |= RobotState::REVERSING_RIGHT;

  // Set the collision bits.
  if(m_distanceL < m_collisionThreshold) state |= RobotState::LEFT_COLL;
  if(m_distanceR < m_collisionThreshold) state |= RobotState::RIGHT_COLL;
  if(m_distanceF < m_collisionThreshold) state |= RobotState::FRONT_COLL;

  // Set the IR alignment bits.
  // if(m_irX.onlyContainsChar(X_CHAR_GOAL)) state |= RobotState::ALIGNED_X;
  // if(m_irY.onlyContainsChar(Y_CHAR_GOAL)) state |= RobotState::ALIGNED_Y;

  // Set the servo arm bits.
  // int salinityArmAngle = m_salinityArm->arm.read();
  // if(salinityArmAngle == SalinityArm::UP_ANGLE) { state |= RobotState::SALINITY_ARM_UP; }
  // else if(salinityArmAngle == SalinityArm::DOWN_ANGLE) { state |= RobotState::SALINITY_ARM_DOWN; }
  // else { state |= RobotState::SALINITY_ARM_MOVING; }

  m_actionMgr->actionLoop(state, this);

  // if((i % 4) == 0){
  //   m_lcd->clear();
  //   m_lcd->setCursor(0, 0);
  //   m_lcd->print(String(m_distanceF));
  //   m_lcd->setCursor(0, 1);
  //   m_lcd->print(String(m_distanceR));
  // }


  
  now = millis();
  m_chomper.chomp(now);

  // displayState();

  if((i % 16)) {
    m_lcd->clear();
    m_lcd->setCursor(0, 0);
    m_lcd->print("incline reading:");
    m_lcd->setCursor(0, 1);
    m_lcd->print(String(getInclinometerReading()));
  }

  delay(ROBOT_TICKRATE);

  ++i;
}
