#ifndef ROBOT_CONSTANTS_H
#define ROBOT_CONSTANTS_H

#define DEFAULT_COLLISION_THRESHOLD 30.0
#define LEFT_COLLISION_THRESHOLD 40.0
#define RIGHT_COLLISION_THRESHOLD 40.0
#define DEFAULT_TURN_LEFT_DURATION 765
#define DEFAULT_TURN_RIGHT_DURTION 785

namespace Pins {
  enum {    
    // M_FRONTRIGHT = 9,

    // M_FRONTLEFT = 10,
    M_BACKLEFT = 6,
    M_BACKRIGHT = 10,
    
    COLLECTOR_SERVO = 13,
    SALINITY_ARM = 3,

    PINGF_ECHO = 51,
    PINGF_TRIG = 53,

    PINGR_TRIG = 31,
    PINGR_ECHO = 29,

    PINGL_ECHO = 8,
    PINGL_TRIG = 7,

    SALINITY_SENSOR = 14,

    IR_X = 1,
    IR_Y = 2,
  };
};



namespace Ping {
    static const int DELAY1 = 2; // microseconds
    static const int DELAY2 = 10; // microseconds
    static const float SOUND_SPEED = 0.0343f; // 343 m/s in 0.0343 cm/us
};


enum MovementStates: short {
    MS_STATIONARY = 0,
    MS_FORWARD,
    MS_REVERSE,
    MS_TURN_LEFT,
    MS_TURN_RIGHT,
    MS_REVERSE_TURN_LEFT,
    MS_REVERSE_TURN_RIGHT,
};

namespace RobotState {
    enum {
        MOTOR_FL_ON = 1 << 0,         
        MOTOR_FR_ON = 1 << 1, 
        MOTOR_BL_ON = 1 << 2,
        MOTOR_BR_ON = 1 << 3,

        MOTOR_FL_REV = 1 << 4,
        MOTOR_FR_REV = 1 << 5,
        MOTOR_BL_REV = 1 << 6,
        MOTOR_BR_REV = 1 << 7,

        LEFT_COLL = 1 << 8,                
        RIGHT_COLL = 1 << 9,              
        FRONT_COLL = 1 << 10,   
    };
 
    constexpr int ENABLE_MOTORS_MASK = MOTOR_FL_ON | MOTOR_FR_ON | MOTOR_BL_ON | MOTOR_BR_ON;
    constexpr int REVERSE_MOTORS_MASK =  MOTOR_FL_REV | MOTOR_FR_REV | MOTOR_BL_REV | MOTOR_BR_REV; 
    constexpr int ANY_COLLISION_MASK = LEFT_COLL | RIGHT_COLL | FRONT_COLL;
};


enum MotorEnum {
    // FL = 0,
    // FR = 1,
    BL = 0,
    BR = 1,
};

namespace Speeds {
    const int LEFT_FORWARD = 115;
    const int LEFT_REVERSE = 75;
    const int RIGHT_FORWARD = 75;
    const int RIGHT_REVERSE = 114;
};

#endif