#include "MovementFSM.h"

MovementFSM::MovementFSM(byte FL_pin, byte FR_pin, byte BL_pin, byte BR_pin) { 
  FL = Motor(FL_pin);
  FR = Motor(FR_pin);
  BL = Motor(BL_pin);
  BR = Motor(BR_pin);
  currentState = &StationaryState::getInstance();
}

void MovementFSM::setState(MovementState& newState){
    currentState->exit(this);  // do stuff before we change state
    currentState = &newState;  // actually change states now
    currentState->enter(this); // do stuff after we change state
}
