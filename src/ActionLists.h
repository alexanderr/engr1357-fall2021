#ifndef ACTIONLISTS_H
#define ACTIONLISTS_H
#include "Actions.h"

// TODO: list of actions w/o hardcoding.
const Action maze_mode[] = {
    {Actions::DONT_MOVE, Events::NONE, 0},
    {Actions::MOVE_F, Events::FRONT_COLLISION, 0},
};

#endif