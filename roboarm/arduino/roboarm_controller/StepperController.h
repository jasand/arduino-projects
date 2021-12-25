#ifndef StepperController_h
#define StepperController_h

#include "Arduino.h"

#define STEP_CW 1
#define STEP_CCW 0

#define FULL_STEP 1
#define HALF_STEP 2
#define QUARTER_STEP 4
#define ONEEIGHT_STEP 8

class StepperController {
private:
  void setDirection();

protected:
  byte _negativeDir;
  byte _direction;
  int _mode;
  byte _ms1Pin, _ms2Pin, _enablePin, _dirPin, _stepPin;

public:
  StepperController(byte negativeDir, byte mode, byte ms1Pin, byte ms2Pin, byte enablePin, byte dirPin, byte stepPin);
  void setMode(byte mode);
  void enable();
  void disable();
  void setNegDir();
  void setPosDir();
  boolean isPosDir();
  void step();
};

#endif
