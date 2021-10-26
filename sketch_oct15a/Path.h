#ifndef PATH_H
#define PATH_H

#include "Robot.h"

namespace RobotState {
  enum: unsigned int {
    STATIONARY = 0, // 0
    FORWARD = 1 << 0, // 10
    LEFT_COLL = 1 << 1, // 100
    RIGHT_COLL = 1 << 2,
    TURNING_LEFT = 1 << 3,
    TURNING_RIGHT = 1 << 4,
    REVERSING_LEFT = 1 << 5,
    REVERSING_RIGHT = 1 << 6,
    ALIGNED_X = 1 << 7,
    ALIGNED_Y = 1 << 8,
  };
};



#define BRIDGE_TIME 5000
#define TURN90_TIME 5000
#define REVERSETURN1_TIME 1000
#define TURNPADDING1_TIME 750

#define REVERSETURN2_TIME 1000
#define TURNPADDING2_TIME 750

#define TURNPADDING3_TIME 3000
#define REVERSETURN3_TIME 1000

#define AFTER_TURN_90(_NEXT_STATE) if(delta > TURN90_TIME) setState(_NEXT_STATE, now); break;


struct Path {
  enum {
    NULL_STATE = 0,
    BRIDGE_STATE,
    ROTATE1_STATE,
    TO_WALL1_STATE,
    REVERSETURN1_STATE,
    FOLLOW_WALL1_STATE,
    CREATE_TURNPADDING1_STATE, // to avoid hitting the wall when turning
    ROTATE2_STATE, // going around 1st wall
    TO_WALL2_STATE, // going to 2nd wall
    REVERSETURN2_STATE,
    FOLLOW_WALL2_STATE,
    CREATE_TURNPADDING2_STATE, // to avoid hitting the wall when turning
    ROTATE3_STATE,
    CREATE_TURNPADDING3_STATE, // to avoid hitting the wall when turning by going forward
    ROTATE4_STATE,
    ALIGN_X_STATE,
    ROTATE5_STATE,
    ALIGN_Y_STATE,
    REVERSE_TURN_PADDING_STATE,
    REVERSETURN3_STATE,
    COLLECT_STATE,
  };
  
  Robot* robot;
  unsigned int state;
  unsigned long ts;

  Path(): robot(nullptr), state(NULL_STATE), ts(0) {}

  void setState(int newState, unsigned long now);

  void loop(unsigned long now);
};

#endif
