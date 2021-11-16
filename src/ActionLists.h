#ifndef ACTIONLISTS_H
#define ACTIONLISTS_H
#include "Actions.h"

// TODO: list of actions w/o hardcoding.
Action maze_mode[] = {
    {Actions::DONT_MOVE, Events::NONE, 0},
    {Actions::MOVE_F, Events::NO_RIGHT_COLLISION},
    {Actions::DONT_MOVE, Events::NONE, 0},
    {Actions::TERMINATE, Events::NONE, 0},
};

#endif