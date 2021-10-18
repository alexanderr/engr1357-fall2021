#ifndef MOVEMENTSTATES_H
#define MOVEMENTSTATES_H

class MovementFSM;

class MovementState {
public:
    virtual void enter(MovementFSM* controller) = 0;
    virtual void exit(MovementFSM* controller) {};
    virtual ~MovementState() {}
};


class StationaryState: public MovementState {
public:
  void enter(MovementFSM* controller);
  static MovementState& getInstance() {
    static StationaryState singleton;
    return singleton;    
  }
};


class ForwardState: public MovementState {
public:
  void enter(MovementFSM* controller);
  static MovementState& getInstance() {
    static ForwardState singleton;
    return singleton;    
  }
};



class ReverseState: public MovementState {
public:
  void enter(MovementFSM* controller);
  static MovementState& getInstance() {
    static ReverseState singleton;
    return singleton;    
  }
};


class TurnLeftState: public MovementState {
public:
  void enter(MovementFSM* controller);
  static MovementState& getInstance() {
    static TurnLeftState singleton;
    return singleton;    
  }
};


class TurnRightState: public MovementState {
public:
  void enter(MovementFSM* controller);
  static MovementState& getInstance() {
    static TurnRightState singleton;
    return singleton;    
  }
};

#endif
