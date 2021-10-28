#include <Arduino.h>
#include "Robot.h"
#include "Actions.h"


Robot* robot;


Action* ActionManager::MAZERIGHT[] = {
    // new TimedForwardAction(4000),
    // new Turn90Action(Directional::LEFT),
    new ToWallAction(30),
    new Turn90Action(Directional::LEFT),
    new FollowWallAction(Directional::RIGHT, 70),
    new TimedForwardAction(1250),
    new Turn90Action(Directional::RIGHT),
    new ToWallAction(30),
    new Turn90Action(Directional::RIGHT),
    new TimedTurnAction(100, Directional::RIGHT),
    new FollowWallAction(Directional::LEFT, 70),
    new TimedForwardAction(750),
    new Turn90Action(Directional::LEFT),
    new TimedForwardAction(3000),
    new Turn90Action(Directional::LEFT),
    // new TimedForwardAction(TURNPADDING4_TIME),
    // new TimedReverseTurnAction(REVERSETURN3_TIME, Directional::LEFT),
    // new CollectAction(COLLECTION_TIME),
};

Action* ActionManager::COLLECT_UP[] = {
    new TimedForwardAction(COLLECT_RETURN_TIME),
};

Action* ActionManager::SALINITYLEFT[] = {
    new FollowWallAction(Directional::LEFT, DEFAULT_COLLISION_THRESHOLD),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::LEFT), 
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::LEFT),
    new AlignIRAction(AlignIRAction::Y),
    new Turn90Action(Directional::RIGHT),  
    new AlignIRAction(AlignIRAction::X),
    new TimedForwardAction(TURNPADDING2_TIME), 
    new ReadInclinationAction(),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::LEFT),
    // new MoveSalinityArmAction(MoveSalinityArmAction::DOWN), 
    new ReadSalinityAction(),
    // new MoveSalinityArmAction(MoveSalinityArmAction::UP), 
};

Action* ActionManager::SALINITYRIGHT[] = {
    new FollowWallAction(Directional::RIGHT, DEFAULT_COLLISION_THRESHOLD),
    new TimedForwardAction(300),
    new Turn90Action(Directional::RIGHT), 
    new TimedForwardAction(3000),
    new Turn90Action(Directional::RIGHT),
    new TimedForwardAction(3000),
    //new AlignIRAction(AlignIRAction::Y),
    new Turn90Action(Directional::LEFT),  
    //new AlignIRAction(AlignIRAction::X),
    new TimedForwardAction(TURNPADDING2_TIME),
    new TimedForwardAction(TURNPADDING2_TIME), 
    new ReadInclinationAction(),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::RIGHT),
    // new MoveSalinityArmAction(MoveSalinityArmAction::DOWN), 
    new ReadSalinityAction(),
    // new MoveSalinityArmAction(MoveSalinityArmAction::UP),
};


Action* ActionManager::MAZELEFT[NUM_MAZEACTIONS] = {
    // new TimedForwardAction(4000),
    // new Turn90Action(Directional::LEFT),
    new ToWallAction(20),
    new Turn90Action(Directional::LEFT),
    new FollowWallAction(Directional::RIGHT, 70),
    new TimedForwardAction(1500),
    new Turn90Action(Directional::RIGHT),
    new ToWallAction(30),
    new Turn90Action(Directional::RIGHT),
    new TimedTurnAction(100, Directional::RIGHT),
    new FollowWallAction(Directional::LEFT, 70),
    new TimedForwardAction(750),
    new Turn90Action(Directional::LEFT),
    new TimedForwardAction(3000),
    new Turn90Action(Directional::LEFT),
};

Action* ActionManager::UNIT_TEST[NUM_TESTACTIONS] = {
    new TimedForwardAction(2000), 
    new StopAction(1000),
    new ReadInclinationAction(),
    new TimedForwardAction(4000), 
    new Turn90Action(Directional::LEFT),
    // new MoveSalinityArmAction(MoveSalinityArmAction::DOWN), 
    new ReadSalinityAction(),
    // new MoveSalinityArmAction(MoveSalinityArmAction::UP),
};

Action* ActionManager::TURN_TEST[1] = {
    new Turn90Action(Directional::RIGHT),
};

Action* ActionManager::TAKE_SALINE[3] = {
    new ToggleSalinityArmAction(),
    new DelayAction(1000),
    new ToggleSalinityArmAction()
};


void setup() {
  robot = new Robot();
  Serial.begin(9600);
  robot->ping();
  robot->m_chomper.m_chomping = true;
}

void loop() {
  robot->loop();
}
