#ifndef ENGR_MOVEMENTFSM_H
#define ENGR_MOVEMENTFSM_H
#include "Motor.h"
#include "MovementStates.h"


class MovementFSM {
public:
    MovementFSM() = default;
    MovementFSM(byte FL_pin, byte FR_pin, byte BL_pin, byte BR_pin);

    void setMovementState(MovementState& newState);

public:
    MovementState* movementState;
    Motor FL;
    Motor FR;
    Motor BL;
    Motor BR;
};



#endif
