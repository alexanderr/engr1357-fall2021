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
      robot->m_controller.setState(ForwardState::getInstance());
      break;
    }

    case ROTATE1_STATE: {
      robot->m_controller.setState(TurnLeftState::getInstance());
      break;
    }

    case REVERSETURN1_STATE: {
      robot->m_controller.setState(ReverseTurnLeftState::getInstance());
      break;
    }

    case ROTATE2_STATE: {
      robot->m_controller.setState(TurnRightState::getInstance());
      break;
    }

    case ROTATE3_STATE: {
      robot->m_controller.setState(TurnLeftState::getInstance());
      break;
    }

    case ROTATE4_STATE: {
      robot->m_controller.setState(TurnLeftState::getInstance());
      break;
    }

    case ROTATE5_STATE: {
      robot->m_controller.setState(TurnRightState::getInstance());
      break;
    }

    case REVERSETURN2_STATE: {
      robot->m_controller.setState(ReverseTurnRightState::getInstance());
      break;
    }

    case REVERSETURN3_STATE: {
      robot->m_controller.setState(ReverseTurnLeftState::getInstance());
      break;
    }
  }
}

void Path::loop(unsigned long now) {
  unsigned long delta = now - ts;
  
  switch(state) {
    case BRIDGE_STATE: {
      //  Fixed time state.
      if(delta > BRIDGE_TIME) {
        // transition to next state.
        setState(ROTATE1_STATE, now);
      }
      break;
    }

    case ROTATE1_STATE: {
      if(delta > TURN90_TIME) {
        // transition to next state.
        setState(TO_WALL1_STATE, now);
      }
      break;
    }
    case TO_WALL1_STATE: {
      if((robot->state & RobotState::LEFT_COLL) && (robot->state & RobotState::RIGHT_COLL)){
        setState(FOLLOW_WALL1_STATE, now);
      }
      break;
    }
    
    case REVERSETURN1_STATE: {
      if(delta > REVERSETURN1_TIME){
        setState(FOLLOW_WALL1_STATE, now);
      }
      break;
    }

    
    case FOLLOW_WALL1_STATE: {
      if(!(robot->state & RobotState::RIGHT_COLL)){
        setState(FOLLOW_WALL1_STATE, now);
      }
      break;
    }

    case CREATE_TURNPADDING1_STATE: {
      if(delta > TURNPADDING1_TIME) setState(FOLLOW_WALL1_STATE, now); break;
    }
    
    case ROTATE2_STATE: {
      if(delta > TO_WALL2_STATE) setState(TO_WALL2_STATE, now); break;
    }
    case TO_WALL2_STATE: {
      if((robot->state & RobotState::LEFT_COLL) && (robot->state & RobotState::RIGHT_COLL)){
        setState(REVERSETURN2_STATE, now);
      }
      break;
    }

    case REVERSETURN2_STATE: {
      if(delta > REVERSETURN2_TIME) setState(FOLLOW_WALL2_STATE, now); break;
    }

    case FOLLOW_WALL2_STATE: {
      if(!(robot->state & RobotState::LEFT_COLL)) setState(CREATE_TURNPADDING2_STATE, now); break;
    }

    case CREATE_TURNPADDING2_STATE: {
      if(delta > TURNPADDING2_TIME) setState(ROTATE3_STATE, now); break;
    }

    case ROTATE3_STATE: {
      if(delta > TURN90_TIME) setState(CREATE_TURNPADDING3_STATE, now); break;
    }

    case CREATE_TURNPADDING3_STATE: {
      if(delta > TURNPADDING3_TIME) setState(ROTATE4_STATE, now); break;
    }

    case ROTATE4_STATE: {
      if(delta > TURN90_TIME) setState(ALIGN_X_STATE, now); break;
    }

    case ALIGN_X_STATE: {
      if(robot->state & RobotState::ALIGNED_X) setState(ROTATE5_STATE, now); break;
    }

    case ROTATE5_STATE: {
      if(delta > TURN90_TIME) setState(ALIGN_Y_STATE, now); break;
    }

    case ALIGN_Y_STATE: {
      if(robot->state & RobotState::ALIGNED_Y) setState(REVERSE_TURN_PADDING_STATE, now); break;
    }

    case REVERSE_TURN_PADDING_STATE: {
      if(delta > TURNPADDING3_TIME) setState(REVERSETURN3_STATE, now); break;
    }

    case REVERSETURN3_STATE: {
      if(delta > REVERSETURN3_TIME) setState(COLLECT_STATE, now); break;
    }
   

  } // end switch
  
}
