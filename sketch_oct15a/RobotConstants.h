#ifndef ROBOTCONSTANTS_H
#define ROBOTCONSTANTS_H

namespace RobotState {
  enum: unsigned short {
    FORWARD = 1 << 0,         // 0000 0000 0000 0001
    TURNING_LEFT = 1 << 1,    // 0000 0000 0000 0010
    TURNING_RIGHT = 1 << 2,   // 0000 0000 0000 0100
    REVERSING_LEFT = 1 << 3,  // 0000 0000 0000 1000
    REVERSING_RIGHT = 1 << 4, // 0000 0000 0001 0000
    LEFT_COLL = 1 << 5,       // 0000 0000 0010 0000
    RIGHT_COLL = 1 << 6,      // 0000 0000 0100 0000
    ALIGNED_X = 1 << 7,       // 0000 0000 1000 0000
    ALIGNED_Y = 1 << 8,       // 0000 0001 0000 0000
  };

  // some useful bitmasks
  const static unsigned short TURNING_MASK = TURNING_LEFT | TURNING_RIGHT;           // 0000 0000 0000 0110
  const static unsigned short REVERSING_MASK = REVERSING_LEFT | REVERSING_RIGHT;     // 0000 0000 0001 1000
  const static unsigned short MOVING_MASK = FORWARD | TURNING_MASK | REVERSING_MASK; // 0000 0000 0001 1111
  const static unsigned short COLLISION_MASK = LEFT_COLL | RIGHT_COLL;               // 0000 0000 0110 0000
   
};


namespace Pins {
  enum {    
    M_FRONTRIGHT = 9,
    M_BACKRIGHT = 5,

    M_FRONTLEFT = 10,
    M_BACKLEFT = 6,
    
    SERVO = 11,

    PING1_ECHO = 29,
    PING1_TRIG = 27,

    PING2_TRIG = 31,
    PING2_ECHO = 33,

    INCLINOMETER = 7,

    IR_X = 1,
    IR_Y = 2,
  };
};



#endif
