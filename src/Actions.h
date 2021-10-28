#ifndef ACTIONS_H
#define ACTIONS_H

#include "RobotConstants.h"
#include "Robot.h"



#define BRIDGE_TIME 5000
#define TURN90_LEFTTIME 1200
#define TURN90_RIGHTTIME 1000
#define STOP_TIME 1000


#define REVERSETURN1_TIME 1000
#define TURNPADDING1_TIME 750

#define REVERSETURN2_TIME 1000
#define TURNPADDING2_TIME 750

#define TURNPADDING3_TIME 3000
#define REVERSETURN3_TIME 1000

#define TURNPADDING4_TIME 3000

#define COLLECT_RETURN_TIME 1500

#define COLLECTION_TIME 5000

#define NUM_MAZEACTIONS 13
#define NUM_SALINITYACTIONS 11
#define NUM_TESTACTIONS 8
#define NUM_SAL_ACTIONS 3


class Robot;

struct Action {
  virtual void doAction(Robot* robot) = 0;
  virtual bool isActionComplete(unsigned long timeDelta, unsigned int robotState) = 0;
  virtual const char* getName() {return "Action"; };
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

struct DelayAction: public TimedAction {
  DelayAction(unsigned int duration): TimedAction(duration) {}

  void doAction(Robot* robot) {};
};

struct TimedDirectionalAction: public TimedAction, public Directional {
  TimedDirectionalAction(unsigned int duration, unsigned char direction): TimedAction(duration), Directional(direction) {}
};

struct Turn90Action: public Action, public Directional {
  Turn90Action(unsigned int direction): Directional(direction) {}
  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
  const char* getName() {return "Turn90Action"; };
};

struct StopAction: public TimedAction {
  StopAction(unsigned int duration): TimedAction(duration) {}
  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
  const char* getName() {return "StopAction"; };
};

struct TimedForwardAction: public TimedAction {
  TimedForwardAction(unsigned int duration): TimedAction(duration) {}
  void doAction(Robot* robot);
  const char* getName() {return "TimedForwardAction"; };
};


struct TimedTurnAction: public TimedDirectionalAction {
  TimedTurnAction(unsigned int duration, unsigned char direction): TimedDirectionalAction(duration, direction) {}
  void doAction(Robot* robot);
  const char* getName() {return "TimedTurnAction"; };
};

struct TimedReverseTurnAction: public TimedDirectionalAction {
  TimedReverseTurnAction(unsigned int duration, unsigned char direction): TimedDirectionalAction(duration, direction) {}
  void doAction(Robot* robot);
  const char* getName() {return "TimedReverseTurnAction"; };
};

struct ToWallAction: public Action {
  float distance;

  ToWallAction(float distance): distance(distance) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
  const char* getName() {return "ToWallAction"; };
};


struct FollowWallAction: public Action, public Directional {
  float distance;
  FollowWallAction(unsigned char direction, float distance): Directional(direction), distance(distance) {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
  const char* getName() {return "FollowWallAction"; };
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


struct ToggleSalinityArmAction: public Action {
  ToggleSalinityArmAction() {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState) { return true; };
};

struct ReadSalinityAction: public Action {
  ReadSalinityAction() = default;

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState);
};


struct StartCollectionAction : public Action {
  StartCollectionAction() {}

  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState) { return true; } 
};

struct StopCollectionAction: public Action {
  StopCollectionAction() {}
  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState) { return true; } 
};

struct SetSpeedAction: public Action {
  int speed;

  SetSpeedAction(int speed): speed(speed) {}
  void doAction(Robot* robot);
  bool isActionComplete(unsigned long timeDelta, unsigned int robotState) { return true; } 
};


struct ActionManager {
  Action** actionArray;
  Action** nextActionArray;

  unsigned short index;
  unsigned short numActions;
  unsigned short nextNumActions;

  unsigned long lastActionStartTime;

  static Action* MAZELEFT[NUM_MAZEACTIONS];
  static Action* MAZERIGHT[NUM_MAZEACTIONS];

  static Action* COLLECT_UP[];
  static Action* COLLECT_DOWN[];
  static Action* COLLECT_LEFT[];
  static Action* COLLECT_RIGHT[];

  static Action* TURN_TEST[];

  static Action* SALINITYLEFT[];
  static Action* SALINITYRIGHT[];

  static Action* TAKE_SALINE[];

  static Action* UNIT_TEST[NUM_TESTACTIONS];


  ActionManager() {
    actionArray = nullptr;
    index = 0;
    numActions = 0;
    lastActionStartTime = 0;
    nextActionArray = nullptr;
  }

  void setActionList(Action** actionList, unsigned short length) {
    actionArray = actionList;
    numActions = length;
    index = 0;
  }

  void actionLoop(unsigned int robotState, Robot* robot);

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
