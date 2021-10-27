#include "MovementStates.h"
#include "Motor.h"
#include "MovementFSM.h"

void StationaryState::enter(MovementFSM* controller) {
  Motor::multiDisable(controller->FL, controller->FR, controller->BL, controller->BR);
}


void TurnLeftState::enter(MovementFSM* controller) {
  Motor::multiSetSpeed(75, controller->FL, controller->BL);
  Motor::multiSetSpeed(75, controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->FR, controller->BL, controller->BR);
}


void TurnRightState::enter(MovementFSM* controller) {
  Motor::multiSetSpeed(113, controller->FL, controller->BL);
  Motor::multiSetSpeed(113, controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->FR, controller->BL, controller->BR);
}

void ForwardState::enter(MovementFSM* controller) {
  Motor::multiSetSpeed(113, controller->FL, controller->BL);
  Motor::multiSetSpeed(75, controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->FR, controller->BL, controller->BR);
}


void ReverseState::enter(MovementFSM* controller) {
  Motor::multiSetSpeed(75, controller->FL, controller->BL);
  Motor::multiSetSpeed(113, controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->FR, controller->BL, controller->BR);
}

void ReverseTurnLeftState::enter(MovementFSM* controller) {
  Motor::multiSetSpeed(75, controller->FL, controller->BL);
  Motor::multiDisable(controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->BL);
}

void ReverseTurnRightState::enter(MovementFSM* controller) {
  Motor::multiDisable(controller->FL, controller->BL);
  Motor::multiSetSpeed(75, controller->FR, controller->BR);
  Motor::multiEnable(controller->FL, controller->FR, controller->BL, controller->BR);
}