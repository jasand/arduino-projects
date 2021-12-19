#ifndef GcodePlotter_h
#define GcodePlotter_h

#include "Arduino.h"
#include "GcodeParser.h"
#include "StepperController.h"
#include <Servo.h>

#define FAST_MOVE        0
#define LIN_INTERP       1
#define CIRC_INTERP_CW   2
#define CIRC_INTERP_CCW  3
#define DWELL            4
#define METRIC_UNITS    21
#define MACHINE_ZERO    28
#define ABSOLUTE_MODE   90
#define RELATIVE_MODE   91
#define END_OF_PROGRAM   2
#define SPINDLE_ON_CW    3
#define SPINDLE_STOP    91

const unsigned long MICROS_PER_MIN = 60000000L;

class GcodePlotter {
private:
  double _maxXmm, _maxYmm, _stepsPerMm;
  double _currentX, _currentY, _currentZ;
  int _currXStep, _currYStep, _maxXStep, _maxYStep;
  unsigned long _feedRate;
  unsigned long _lastMove;
  int _penUpDeg, _penDownDeg;
  byte _endstopXPin, _endstopYPin;
  char _mode;
  struct GcodeCmd gcodeCmd;
  GcodeParser gcodeParser;
  Servo* _pen;
  StepperController* _xMotor;
  StepperController* _yMotor;

  void movePen(double z);
  double calculateRealTarget(double target, double current);
  double calculateAngle(double x, double y, double cx, double cy);
  void stepX();
  void stepY();
  unsigned long stepRateMicros();
  void sleepUntilNextMove();

protected:
  void fastMove(double x, double y, double z, unsigned int feedRate);
  void linearInterpolation(double x, double y, double z, unsigned int feedRate);
  void circularInterpolation(double x, double y, double z, double i, double j, unsigned int feedRate, byte dir);
  //void circularInterpolationAlt(double x, double y, double z, double i, double j, unsigned int feedRate, byte dir);
  
public:
  GcodePlotter(double maxXmm, double maxYmm, double stepsPerMm, int penUpDeg, int penDownDeg, byte endstopXPin, byte endstopYPin,
               Servo* pen, StepperController* xMotor, StepperController* yMotor);
  void processLine(char* line);
  void processGcodeCmd(GcodeCmd* cmd);
  void home();
};

#endif
