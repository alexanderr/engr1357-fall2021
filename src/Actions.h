#ifndef ROBOT_ACTIONS
#define ROBOT_ACTIONS
#include "Events.h"
#include "RobotUtil.h"
#include "Chomper.h"

extern int requested_motor_state;
extern Chomper chomper;
extern bool object_avoidance;

namespace Actions
{
    using ActionFunction = void (*)();

    enum : int
    {
        DONT_MOVE = 0,
        MOVE_F,
        MOVE_R,
        TURN_L,
        TURN_R,
        CHOMP_ON,
        CHOMP_OFF,
        OBJ_AV_OFF,
        OBJ_AV_ON,
        TERMINATE,
        MAX_ACTIONS
    };

    void dont_move() { requested_motor_state = MS_STATIONARY; }
    void move_f() { requested_motor_state = MS_FORWARD; }
    void move_r() { requested_motor_state = MS_REVERSE; }
    void turn_l() { requested_motor_state = MS_TURN_LEFT; }
    void turn_r() { requested_motor_state = MS_TURN_RIGHT; }
    void chomp_on()
    {
        requested_motor_state = MS_FORWARD;
        chomper.m_chomping = true;
    }
    void chomp_off()
    {
        requested_motor_state = MS_STATIONARY;
        chomper.m_chomping = false;
    }

    void obj_avoidance_off() {
        object_avoidance = false;
    }
    void obj_avoidance_on() {
        object_avoidance = true;
    }

    void terminate() {}

    ActionFunction FunctionTable[MAX_ACTIONS] = {
        &dont_move,
        &move_f,
        &move_r,
        &turn_l,
        &turn_r,
        &chomp_on,
        &chomp_off,
        &obj_avoidance_off,
        &obj_avoidance_on,
        &terminate,
    };

};

struct Action
{
    int action;
    int endEvent;
    unsigned long duration;
    Action(int action, int endEvent = Events::NONE, unsigned long duration = 0) : action(action), endEvent(endEvent), duration(duration) {}
};

#endif