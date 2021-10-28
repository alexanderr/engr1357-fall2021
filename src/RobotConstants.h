#ifndef ROBOTCONSTANTS_H
#define ROBOTCONSTANTS_H

namespace RobotState {
  enum: unsigned short {
    FORWARD = 1 << 0,                 // 0000 0000 0000 0001
    TURNING_LEFT = 1 << 1,            // 0000 0000 0000 0010
    TURNING_RIGHT = 1 << 2,           // 0000 0000 0000 0100
    REVERSING_LEFT = 1 << 3,          // 0000 0000 0000 1000
    REVERSING_RIGHT = 1 << 4,         // 0000 0000 0001 0000
    LEFT_COLL = 1 << 5,               // 0000 0000 0010 0000
    RIGHT_COLL = 1 << 6,              // 0000 0000 0100 0000
    FRONT_COLL = 1 << 7,              // 0000 0000 1000 0000
    ALIGNED_X = 1 << 7,               // 0000 0001 0000 0000
    ALIGNED_Y = 1 << 8,               // 0000 0010 0000 0000
    SALINITY_ARM_UP = 1 << 9,         // 0000 0100 0000 0000
    SALINITY_ARM_DOWN = 1 << 10,      // 0000 1000 0000 0000
    SALINITY_ARM_MOVING = 1 << 11,    // 0001 0000 0000 0000
    TAKING_INCLINE = 1 << 12,
  };

  // some useful bitmasks
  const static unsigned short TURNING_MASK = TURNING_LEFT | TURNING_RIGHT;           // 0000 0000 0000 0110
  const static unsigned short REVERSING_MASK = REVERSING_LEFT | REVERSING_RIGHT;     // 0000 0000 0001 1000
  const static unsigned short MOVING_MASK = FORWARD | TURNING_MASK | REVERSING_MASK; // 0000 0000 0001 1111
  // const static unsigned short COLLISION_MASK = LEFT_COLL | RIGHT_COLL;               // 0000 0000 0110 0000
   
};


namespace Pins {
  enum {    
    M_FRONTRIGHT = 9,
    M_BACKRIGHT = 5,

    M_FRONTLEFT = 10,
    M_BACKLEFT = 6,
    
    COLLECTOR_SERVO = 11,
    SALINITY_ARM = 3,

    PINGF_ECHO = 29,
    PINGF_TRIG = 27,

    PINGR_TRIG = 23,
    PINGR_ECHO = 25,

    PINGL_ECHO = 13,
    PINGL_TRIG = 12,

    INCLINOMETER = 0,

    SALINITY_SENSOR = 14,

    IR_X = 1,
    IR_Y = 2,
  };
};

namespace RobotUtil {

template<typename T>
void insertionSort(T array[], int size) {
  for (int step = 1; step < size; step++) {
    int key = array[step];
    int j = step - 1;

    while (key < array[j] && j >= 0) {
      array[j + 1] = array[j];
      --j;
    }
    array[j + 1] = key;
  }
}
}



#endif
