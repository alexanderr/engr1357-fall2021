#include "Path.h"


void Path::setState(int newState, unsigned long now)  {
  state = newState;
  ts = now;

  switch(state) {
    case BRIDGE_STATE:
    case TO_WALL1_STATE:
    case TO_WALL2_STATE:
    case ALIGN_X_STATE:
    case ALIGN_Y_STATE:
    case CREATE_TURNPADDING1_STATE:
    case CREATE_TURNPADDING3_STATE:
    case CREATE_TURNPADDING2_STATE:
    case FOLLOW_WALL1_STATE:
    case FOLLOW_WALL2_STATE:
    case REVERSE_TURN_PADDING_STATE:
    {
      robot->setMovementState(ForwardState::getInstance()); break;
    }

    case ROTATE1_STATE: { robot->setMovementState(TurnLeftState::getInstance()); break; }

    case REVERSETURN1_STATE: { robot->setMovementState(ReverseTurnLeftState::getInstance()); break; }

    case ROTATE2_STATE: { robot->setMovementState(TurnRightState::getInstance()); break; }

    case ROTATE3_STATE: { robot->setMovementState(TurnLeftState::getInstance()); break;}

    case ROTATE4_STATE: { robot->setMovementState(TurnLeftState::getInstance()); break; }

    case ROTATE5_STATE: { robot->setMovementState(TurnRightState::getInstance()); break; }

    case REVERSETURN2_STATE: { robot->setMovementState(ReverseTurnRightState::getInstance()); break; }

    case REVERSETURN3_STATE: { robot->setMovementState(ReverseTurnLeftState::getInstance()); break; }

    case COLLECT_STATE: {
      robot->setMovementState(StationaryState::getInstance()); break;
    }
  }
}

void Path::loop(unsigned long now) {
  unsigned long delta = now - ts;


  // state transition table
  // yes, this is a mess.
  switch(state) {
    case BRIDGE_STATE: if(delta > BRIDGE_TIME) setState(ROTATE1_STATE, now); break;
    case ROTATE1_STATE: if(delta > TURN90_TIME) setState(TO_WALL1_STATE, now); break;
    case TO_WALL1_STATE: if(robot->hasFrontCollision()) setState(FOLLOW_WALL1_STATE, now); break;
    case REVERSETURN1_STATE: if(delta > REVERSETURN1_TIME) setState(FOLLOW_WALL1_STATE, now); break;
    case FOLLOW_WALL1_STATE: if(!robot->hasRightCollision()) setState(FOLLOW_WALL1_STATE, now); break;
    case CREATE_TURNPADDING1_STATE: if(delta > TURNPADDING1_TIME) setState(FOLLOW_WALL1_STATE, now); break;
    case ROTATE2_STATE: if(delta > TURN90_TIME) setState(TO_WALL2_STATE, now); break;
    case TO_WALL2_STATE: if(robot->hasFrontCollision()) setState(REVERSETURN2_STATE, now); break;
    case REVERSETURN2_STATE: if(delta > REVERSETURN2_TIME) setState(FOLLOW_WALL2_STATE, now); break;
    case FOLLOW_WALL2_STATE: if(!robot->hasLeftCollision()) setState(CREATE_TURNPADDING2_STATE, now); break;
    case CREATE_TURNPADDING2_STATE: if(delta > TURNPADDING2_TIME) setState(ROTATE3_STATE, now); break;
    case ROTATE3_STATE: if(delta > TURN90_TIME) setState(CREATE_TURNPADDING3_STATE, now); break;
    case CREATE_TURNPADDING3_STATE: if(delta > TURNPADDING3_TIME) setState(ROTATE4_STATE, now); break;
    case ROTATE4_STATE: if(delta > TURN90_TIME) setState(ALIGN_X_STATE, now); break;
    case ALIGN_X_STATE: if(robot->state & RobotState::ALIGNED_X) setState(ROTATE5_STATE, now); break;
    case ROTATE5_STATE: if(delta > TURN90_TIME) setState(ALIGN_Y_STATE, now); break;
    case ALIGN_Y_STATE: if(robot->state & RobotState::ALIGNED_Y) setState(REVERSE_TURN_PADDING_STATE, now); break;
    case REVERSE_TURN_PADDING_STATE: if(delta > TURNPADDING3_TIME) setState(REVERSETURN3_STATE, now); break;
    case REVERSETURN3_STATE: if(delta > REVERSETURN3_TIME) setState(COLLECT_STATE, now); break;
  } // end switch
  
}
