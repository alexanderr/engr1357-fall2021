#ifndef ACTIONS_H
#define ACTIONS_H

#include "RobotConstants.h"
#include "Robot.h"



#define BRIDGE_TIME 5000
#define TURN90_TIME 5000
#define REVERSETURN1_TIME 1000
#define TURNPADDING1_TIME 750

#define REVERSETURN2_TIME 1000
#define TURNPADDING2_TIME 750

#define TURNPADDING3_TIME 3000
#define REVERSETURN3_TIME 1000

#define TURNPADDING4_TIME 3000

#define COLLECT_RETURN_TIME 1500

#define COLLECTION_TIME 5000



class Robot;

struct Action {
  virtual void doAction(Robot* robot) = 0;
  virtual bool isActionComplete(unsigned long timeDelta, unsigned int robotState) = 0;
};

struct Directional {
   enum: unsigned char {
    LEFT,
    RIGHT
  };
  unsigned char direction;
  Directional(unsigned char direction): direction(direction) {}
};

struct TimedAction: public Action {
  unsigned int duration;
  TimedAction(unsigned int duration): duration(duration) {}
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState) {
    return timeDelta >= duration;
  };
};

struct TimedDirectionalAction: public TimedAction, public Directional {
  TimedDirectionalAction(unsigned int duration, unsigned char direction): TimedAction(duration), Directional(direction) {}
};

struct Turn90Action: public TimedAction, public Directional {
  Turn90Action(unsigned int direction): TimedAction(TURN90_TIME), Directional(direction) {}
  void doAction(Robot* robot);
};

struct TimedForwardAction: public TimedAction {
  TimedForwardAction(unsigned int duration): TimedAction(duration) {}

  void doAction(Robot* robot);
};


struct TimedTurnAction: public TimedDirectionalAction {
  TimedTurnAction(unsigned int duration, unsigned char direction): TimedDirectionalAction(duration, direction) {}
  void doAction(Robot* robot);
};

struct TimedReverseTurnAction: public TimedDirectionalAction {
  TimedReverseTurnAction(unsigned int duration, unsigned char direction): TimedDirectionalAction(duration, direction) {}
  void doAction(Robot* robot);
};

struct ToWallAction: public Action {
  float distance;

  ToWallAction(float distance): distance(distance) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};


struct FollowWallAction: public Action, public Directional {
  FollowWallAction(unsigned char direction): Directional(direction) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};

struct AlignIRAction: public Action {
  enum {
    X,
    Y
  };

  unsigned char IRType;
  AlignIRAction(unsigned char IRType): IRType(IRType) {} 

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};

struct ReadInclinationAction: public Action {
  ReadInclinationAction() = default;

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};


struct MoveSalinityArmAction: public Action {
  enum {
    UP,
    DOWN
  };

  unsigned char direction;

  MoveSalinityArmAction(unsigned char direction): direction(direction) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};

struct ReadSalinityAction: public Action {
  ReadSalinityAction() = default;

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};

struct CollectAction: public Action {
  unsigned int duration;
  CollectAction(unsigned int duration): duration(duration) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};


struct ActionManager {
  Action** actionArray;
  unsigned short index;
  unsigned short numActions;
  unsigned long lastActionStartTime;

  static Action* MAZELEFT[];
  static Action* MAZERIGHT[];
  static Action* MAZERETURNLEFT[];
  static Action* MAZERETURNRIGHT[];
  static Action* SALINITYLEFT[];
  static Action* SALINITYRIGHT[];
  static Action* SALINITYRETURNLEFT[];
  static Action* SALINITYRETURNRIGHT[];

  ActionManager() {
    actionArray = nullptr;
    index = 0;
    numActions = 0;
    lastActionStartTime = 0;
  }

  void setActionList(Action** actionList) {
    actionArray = actionList;
    numActions = sizeof(actionArray) / sizeof(Action*);
  }

  void actionLoop(unsigned long now, unsigned int robotState, Robot* robot) {
    // Check if we have actions to complete.
    if(actionArray == nullptr || (index >= numActions))
      return;

    if(lastActionStartTime == 0){
      // Start the first action.
      lastActionStartTime = now;
      actionArray[index]->doAction(robot);
    }
    else {
      Action* currentAction = actionArray[index];
      unsigned long timeDelta = lastActionStartTime - now;
      if(currentAction->isActionComplete(timeDelta, robotState)){
        // Start a new action.
        Action* currentAction = actionArray[++index];
        lastActionStartTime = now;
        currentAction->doAction(robot);
      }
    }
  }

  Action** getPathFromCode(const char* code) {
    if(strcmp(code, "A1") == 0){
      return MAZELEFT;
    }
    else if(strcmp(code, "A2") == 0){
      return MAZELEFT;
    }
    else if(strcmp(code, "B1") == 0){
      return SALINITYLEFT;
    }
    else if(strcmp(code, "B2") == 0){
      return SALINITYRIGHT;
    }
    else {
      return nullptr;
    }
  }
};

#endif
