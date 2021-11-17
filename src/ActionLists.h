#ifndef ACTIONLISTS_H
#define ACTIONLISTS_H
#include "Actions.h"
#include <stdlib.h>

// TODO: list of actions w/o hardcoding.
Action maze_mode[] = {
    {Actions::DONT_MOVE, Events::NONE, 1000},
    {Actions::MOVE_F, Events::NONE, 5000},
    {Actions::TURN_L, Events::NONE, DEFAULT_TURN_LEFT_DURATION},
    {Actions::MOVE_F, Events::FRONT_COLLISION},
    {Actions::TURN_L, Events::NONE, DEFAULT_TURN_LEFT_DURATION},
    {Actions::MOVE_F, Events::NO_RIGHT_COLLISION},
    {Actions::MOVE_F, Events::NONE, 500},
    {Actions::TURN_R, Events::NONE, DEFAULT_TURN_RIGHT_DURTION},
    {Actions::MOVE_F, Events::FRONT_COLLISION},
    {Actions::TURN_R, Events::NONE, DEFAULT_TURN_RIGHT_DURTION},
    {Actions::MOVE_F, Events::NO_LEFT_COLLISION},
    {Actions::MOVE_F, Events::NONE, 500},
    {Actions::TURN_L, Events::NONE, DEFAULT_TURN_LEFT_DURATION},
    {Actions::MOVE_F, Events::NONE, 500},
    {Actions::DONT_MOVE, Events::NONE, 0},
    {Actions::TERMINATE, Events::NONE, 0},
};

#endif