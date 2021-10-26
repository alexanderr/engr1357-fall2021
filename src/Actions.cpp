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