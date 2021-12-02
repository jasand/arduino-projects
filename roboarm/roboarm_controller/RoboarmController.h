#ifndef RoboarmController_h
#define RoboarmController_h

#include <Servo.h>
#include "StepperController.h"
#include "CommandParser.h"

class RoboarmController {
private:
  int _currJA, _currJB, _currJC, _currJD, _currJE, _currJF;
  int _lowA = 0, _highA = 180, _lowB = 0, _highB = 180, _lowC = 0, _highC = 180;
  int _lowD = 0, _highD = 180, _lowE = 0, _highE = 180, _lowF = 0, _highF = 180;
  CommandParser parser;
  Command cmd;
  StepperController* _jointA;  // Base
  StepperController* _jointB;
  Servo* _jointC;
  Servo* _jointD;
  Servo* _jointE;    // Wrist
  Servo* _jointF;    // Gripper
  byte _endstopA, _endstopB;

protected:
  void moveArm(int ja, int jb, int jc, int jd, int je, int jf);
  void printPosition();
  void printLimits();

public:
  RoboarmController(StepperController* jA, StepperController* jB, Servo* jC, Servo* jD, Servo* jE, Servo* jF, byte endstopA, byte endstopB);
  void processCommand(char* line);
  void zero();
};

#endif
