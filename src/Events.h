#ifndef ROBOT_EVENTS_H
#define ROBOT_EVENTS_H


namespace Events {
    enum: int {
        NONE = 0,
        NO_COLLISION = 1,
        FRONT_COLLISION = 2,
        LEFT_COLLISION = 3,
        RIGHT_COLLISION = 4,
        ANY_COLLISION = 5,
        INCLINE_CHANGE = 6,
        ACTION_TIMEOUT = 7,
        NO_LEFT_COLLISION = 8,
        NO_RIGHT_COLLISION = 9,
        NO_FRONT_COLLISION = 10,
    };
};

#endif