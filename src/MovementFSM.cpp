#include "MovementFSM.h"

MovementFSM::MovementFSM(byte FL_pin, byte FR_pin, byte BL_pin, byte BR_pin) { 
  FL = Motor(FL_pin);
  FR = Motor(FR_pin);
  BL = Motor(BL_pin);
  BR = Motor(BR_pin);
  movementState = &StationaryState::getInstance();
}

void MovementFSM::setMovementState(MovementState& newState){
    movementState->exit(this);  // do stuff before we change state
    movementState = &newState;  // actually change states now
    movementState->enter(this); // do stuff after we change state
}
