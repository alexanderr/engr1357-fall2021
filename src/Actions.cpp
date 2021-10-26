#include "Actions.h"

void Turn90Action::doAction(Robot* robot) {
  if(direction == LEFT)
    robot->setMovementState(TurnLeftState::getInstance());
  else
    robot->setMovementState(TurnRightState::getInstance());
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
  return (robotState & RobotState::COLLISION_MASK);
}

void FollowWallAction::doAction(Robot* robot) {
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