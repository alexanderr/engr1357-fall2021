#ifndef ROBOTSTATES_H
#define ROBOTSTATES_H

class Robot;

class RobotState {
public:
    virtual void enter(Robot* robot) = 0;
    virtual void exit(Robot* robot) {};
    virtual ~RobotState() {}
};


class AlignYState: public RobotState {
public:
  void enter(Robot* robot);
  static RobotState& getInstance() {
    static AlignYState singleton;
    return singleton;    
  }
};

#endif
