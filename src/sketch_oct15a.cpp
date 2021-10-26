#include <Arduino.h>
#include "Robot.h"
#include "Actions.h"


Robot* robot;

Action* ActionManager::MAZELEFT[] = {
    new TimedForwardAction(BRIDGE_TIME),
    new Turn90Action(Directional::LEFT),
    new ToWallAction(DEFAULT_COLLISION_THRESHOLD),
    new TimedReverseTurnAction(REVERSETURN1_TIME, Directional::RIGHT),
    new FollowWallAction(Directional::RIGHT),
    new TimedForwardAction(TURNPADDING1_TIME),
    new Turn90Action(Directional::RIGHT),
    new ToWallAction(DEFAULT_COLLISION_THRESHOLD),
    new TimedReverseTurnAction(REVERSETURN2_TIME, Directional::LEFT),
    new FollowWallAction(Directional::LEFT),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::LEFT),
    new TimedForwardAction(TURNPADDING3_TIME),
    new Turn90Action(Directional::LEFT),
    new AlignIRAction(AlignIRAction::X),
    new Turn90Action(Directional::RIGHT),
    new AlignIRAction(AlignIRAction::Y),
    new TimedForwardAction(TURNPADDING4_TIME),
    new TimedReverseTurnAction(REVERSETURN3_TIME, Directional::RIGHT),
    new CollectAction(COLLECTION_TIME),
};

Action* ActionManager::MAZERIGHT[] = {
    new TimedForwardAction(BRIDGE_TIME),
    new Turn90Action(Directional::RIGHT),
    new ToWallAction(DEFAULT_COLLISION_THRESHOLD),
    new TimedReverseTurnAction(REVERSETURN1_TIME, Directional::LEFT),
    new FollowWallAction(Directional::LEFT),
    new TimedForwardAction(TURNPADDING1_TIME),
    new Turn90Action(Directional::LEFT),
    new ToWallAction(DEFAULT_COLLISION_THRESHOLD),
    new TimedReverseTurnAction(REVERSETURN2_TIME, Directional::RIGHT),
    new FollowWallAction(Directional::RIGHT),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::RIGHT),
    new TimedForwardAction(TURNPADDING3_TIME),
    new Turn90Action(Directional::RIGHT),
    new AlignIRAction(AlignIRAction::X),
    new Turn90Action(Directional::LEFT),
    new AlignIRAction(AlignIRAction::Y),
    new TimedForwardAction(TURNPADDING4_TIME),
    new TimedReverseTurnAction(REVERSETURN3_TIME, Directional::LEFT),
    new CollectAction(COLLECTION_TIME),
};

Action* ActionManager::SALINITYLEFT[] = {
    new FollowWallAction(Directional::LEFT),
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
    new Turn90Action(Directional::RIGHT),
    new MoveSalinityArmAction(MoveSalinityArmAction::DOWN), 
    new ReadSalinityAction(),
    new MoveSalinityArmAction(MoveSalinityArmAction::UP), 
};

Action* ActionManager::SALINITYRIGHT[] = {
    new FollowWallAction(Directional::RIGHT),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::RIGHT), 
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::RIGHT),
    new AlignIRAction(AlignIRAction::Y),
    new Turn90Action(Directional::LEFT),  
    new AlignIRAction(AlignIRAction::X),
    new TimedForwardAction(TURNPADDING2_TIME), 
    new ReadInclinationAction(),
    new TimedForwardAction(TURNPADDING2_TIME),
    new Turn90Action(Directional::LEFT),
    new MoveSalinityArmAction(MoveSalinityArmAction::DOWN), 
    new ReadSalinityAction(),
    new MoveSalinityArmAction(MoveSalinityArmAction::UP),
};

void setup() {
  robot = new Robot();
  Serial.begin(9600);
}

void loop() {
  robot->loop();
}
