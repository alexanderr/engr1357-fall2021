#include "Actions.h"

bool Turn90Action::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
  if(direction == LEFT)
    return timeDelta >= TURN90_LEFTTIME;
  else
    return timeDelta >= TURN90_RIGHTTIME;
}

void Turn90Action::doAction(Robot* robot) {
  if(direction == LEFT)
    robot->setMovementState(TurnLeftState::getInstance());
  else
    robot->setMovementState(TurnRightState::getInstance());
}

bool StopAction::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
    return timeDelta >= STOP_TIME;
}

void StopAction::doAction(Robot* robot) {
    robot->setMovementState(StationaryState::getInstance());
}

void TimedForwardAction::doAction(Robot* robot) {
  robot->setMovementState(ForwardState::getInstance());
}

void TimedTurnAction::doAction(Robot* robot) {
  if(direction == LEFT)
    robot->setMovementState(TurnLeftState::getInstance());
  else
    robot->setMovementState(TurnRightState::getInstance());
}

void TimedReverseTurnAction::doAction(Robot* robot) {
  if(direction == LEFT)
    robot->setMovementState(ReverseTurnLeftState::getInstance());
  else
    robot->setMovementState(ReverseTurnRightState::getInstance());
}

void ToWallAction::doAction(Robot* robot) {
  robot->m_collisionThreshold = distance;
  robot->setMovementState(ForwardState::getInstance());
}

bool ToWallAction::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
  return (robotState & RobotState::FRONT_COLL);
}

void FollowWallAction::doAction(Robot* robot) {
  robot->m_collisionThreshold = distance;
  robot->setMovementState(ForwardState::getInstance());
}

bool FollowWallAction::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
  if(direction == LEFT)
    return !(robotState & RobotState::LEFT_COLL);
  else
    return !(robotState & RobotState::RIGHT_COLL);
}

void AlignIRAction::doAction(Robot* robot) {
  robot->setMovementState(ForwardState::getInstance());
}

bool AlignIRAction::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
  if(IRType == X)
    return robotState & RobotState::ALIGNED_X;
  else
    return robotState & RobotState::ALIGNED_Y;
}

void ReadInclinationAction::doAction(Robot* robot) {
  robot->getInclinometerReading();
}

bool ReadInclinationAction::isActionComplete(unsigned long timeDelta, unsigned int robotState){
  return true;
}

void ReadSalinityAction::doAction(Robot* robot) {
  robot->getSalinityReading();
}

bool ReadSalinityAction::isActionComplete(unsigned long timeDelta, unsigned int robotState){
  return true;
}

void MoveSalinityArmAction::doAction(Robot* robot) {
  if(direction == UP){
    robot->m_salinityArm->moveUp();
  } else {
    robot->m_salinityArm->moveDown();
  }
}

bool MoveSalinityArmAction::isActionComplete(unsigned long timeDelta, unsigned int robotState){
  if ((robotState & RobotState::SALINITY_ARM_UP) && (direction == UP)) {
    return true;
  } else if ((robotState & RobotState::SALINITY_ARM_DOWN) && (direction == DOWN)) {
    return true;
  } else {
    return false;
  }
}

void CollectAction::doAction(Robot* robot) {
  robot->m_chomper.m_chomping = true;
}

bool CollectAction::isActionComplete(unsigned long timeDelta, unsigned int robotState) {
  return timeDelta > duration;
}

void StartCollectionAction::doAction(Robot* robot) {

}


void ActionManager::actionLoop(unsigned int robotState, Robot* robot) {
    unsigned long now = millis();
    // Check if we have actions to complete.
    if(actionArray == nullptr)
      return;
    
    if(index >= numActions){
      if(nextActionArray != nullptr){
         setActionList(nextActionArray, nextNumActions);
         nextActionArray = nullptr;
         nextNumActions = 0;
         lastActionStartTime = 0;
      }
      else{
        return;
      }
    }

    if(lastActionStartTime == 0){
      // Start the action.
      lastActionStartTime = now;
      actionArray[index]->doAction(robot);
      Serial.println(String("Started action:") + String(index));
    }
    else {
      Action* currentAction = actionArray[index];
      unsigned long timeDelta = now - lastActionStartTime;
      Serial.println("timeDelta:" + String(timeDelta));
      if(currentAction->isActionComplete(timeDelta, robotState)){
        Serial.println(String("Action complete! :") + String(index));
        ++index;
        lastActionStartTime = 0;
        if(index == numActions)
          robot->setMovementState(StationaryState::getInstance());
      }
    }
}
