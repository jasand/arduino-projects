#include "GcodePlotter.h"
#include <math.h> 

GcodePlotter::GcodePlotter(double maxXmm, double maxYmm, double stepsPerMm, int penUpDeg, int penDownDeg, byte endstopXPin, byte endstopYPin,
             Servo* pen, StepperController* xMotor, StepperController* yMotor) {
  _maxXmm = maxXmm;
  _maxYmm = maxYmm;
  _stepsPerMm = stepsPerMm;
  _penUpDeg = penUpDeg;
  _penDownDeg = penDownDeg;
  _pen = pen;
  _endstopXPin = endstopXPin;
  _endstopYPin = endstopYPin;
  _xMotor = xMotor;
  _yMotor = yMotor;
  _mode = ABSOLUTE_MODE;
  _maxXStep = round(_maxXmm * _stepsPerMm);
  _maxYStep = round(_maxYmm * _stepsPerMm);
  _feedRate = 3000;
  _lastMove = 0;
}

void GcodePlotter::processLine(char* line) {
  gcodeParser.parseLine(line, &gcodeCmd);
  //gcodeParser.printCommand(gcodeCmd);
  processGcodeCmd(&gcodeCmd);
}

void GcodePlotter::processGcodeCmd(GcodeCmd* cmd) {
  switch (cmd->cmdType) {
    case GCMD:
      switch (cmd->cmdNum) {
        case FAST_MOVE:
          fastMove(cmd->xParam, cmd->yParam, cmd->zParam, cmd->fParam);
          break;
        case LIN_INTERP:
          linearInterpolation(cmd->xParam, cmd->yParam, cmd->zParam, cmd->fParam);
          break;
        case CIRC_INTERP_CW:
          circularInterpolation(cmd->xParam, cmd->yParam, cmd->zParam, cmd->iParam, cmd->jParam, cmd->fParam, CIRC_INTERP_CW);
          break;
        case CIRC_INTERP_CCW:
          circularInterpolation(cmd->xParam, cmd->yParam, cmd->zParam, cmd->iParam, cmd->jParam, cmd->fParam, CIRC_INTERP_CCW);
          break;
        case MACHINE_ZERO:
          home();
          break;
        case ABSOLUTE_MODE:
          _mode = ABSOLUTE_MODE;
          //Serial.print("Setting mode: ");
          //Serial.println((int)_mode);
          break;
        case RELATIVE_MODE:
          _mode = RELATIVE_MODE;
          //Serial.print("Setting mode: ");
          //Serial.println((int)_mode);
          break;
        case METRIC_UNITS:
          break;
        //default:
          //Serial.print("Command not recognized: ");
          //Serial.println(cmd->cmdNum);
      }
      Serial.println("ok");
      break;
    case MCMD:
      // xxx
      break;
    default:
      //Serial.println("Not a valid command");
      Serial.println("error: Not a valid command");
  }
}

void GcodePlotter::home() {
  // TODO: home
  //Serial.println("HOME...");
  
  // Z
  movePen(1.0);
  
  // X
  _xMotor->setNegDir();
  while(digitalRead(_endstopXPin) == LOW) {
    _xMotor->step();
    delay(1);
  }
  _xMotor->setPosDir();
  _xMotor->step();

  // Y
  _yMotor->setPosDir();
  while(digitalRead(_endstopYPin) == LOW) {
    _yMotor->step();
    delay(1);
  }
  _yMotor->setNegDir();
  _yMotor->step();
  // Endstop switch on max pos Y, so step to y=0
  for(int i=0; i < _maxYmm  * _stepsPerMm ; i++) {
    _yMotor->step();
    delay(1);
  }
  
  _currentX = 0.0;
  _currentY = 0.0;
  _currXStep = 0;
  _currYStep = 0;
}

void GcodePlotter::fastMove(double x, double y, double z, unsigned int feedRate) {
  double targetX, targetY, targetZ;
  if (_mode == ABSOLUTE_MODE) {
    targetX = x != UNDEF_DOUBLE ? x : _currentX;;
    targetY = y != UNDEF_DOUBLE ? y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? z : _currentZ;
  } else {
    targetX = x != UNDEF_DOUBLE ? _currentX + x : _currentX;
    targetY = y != UNDEF_DOUBLE ? _currentY + y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? _currentZ + z : _currentZ;
  }

  if (feedRate != UNDEF_INTEGER) {
    _feedRate = feedRate; // Set feedrate, but ignored in this method...
  }
  
  // Z first
  if (targetZ != _currentZ) {
    movePen(targetZ);
  }
  // Move XY
  int stepsX = round(targetX * _stepsPerMm) - round(_currentX * _stepsPerMm);
  int stepsY = round(targetY * _stepsPerMm) - round(_currentY * _stepsPerMm);
  if (stepsX < 0) {
    _xMotor->setNegDir();
    stepsX = -stepsX;
  } else {
    _xMotor->setPosDir();
  }
  if (stepsY < 0) {
    _yMotor->setNegDir();
    stepsY = -stepsY;
  } else {
    _yMotor->setPosDir();
  }
  int steps = stepsX - stepsY < 0 ? stepsY : stepsX;
  while (steps > 0) {
    if (stepsX > 0) stepX();
    if (stepsY > 0) stepY();
    steps--;
    stepsX--;
    stepsY--;
    delay(1);
  }
  _currentX = targetX;
  _currentY = targetY;
}

void GcodePlotter::linearInterpolation(double x, double y, double z, unsigned int feedRate) {
  double targetX, targetY, targetZ;
  if (_mode == ABSOLUTE_MODE) {
    targetX = x != UNDEF_DOUBLE ? x : _currentX;;
    targetY = y != UNDEF_DOUBLE ? y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? z : _currentZ;
  } else {
    targetX = x != UNDEF_DOUBLE ? _currentX + x : _currentX;
    targetY = y != UNDEF_DOUBLE ? _currentY + y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? _currentZ + z : _currentZ;
  }

  if (feedRate != UNDEF_INTEGER) {
    _feedRate = feedRate; // Set feedrate
  }
  
  // Z first
  if (targetZ != _currentZ) {
    movePen(targetZ);
  }
  // Move XY
  int stepsX = round(targetX * _stepsPerMm) - round(_currentX * _stepsPerMm);
  int stepsY = round(targetY * _stepsPerMm) - round(_currentY * _stepsPerMm);
  if (stepsX < 0) {
    _xMotor->setNegDir();
    stepsX = -stepsX;
  } else {
    _xMotor->setPosDir();
  }
  if (stepsY < 0) {
    _yMotor->setNegDir();
    stepsY = -stepsY;
  } else {
    _yMotor->setPosDir();
  }

  if (stepsX >= stepsY) {
    int lastYStep = 0;
    for (int i=1; i<=stepsX; i++) {
      sleepUntilNextMove();
      stepX();
      if (round((double)stepsY * (double)i / (double)stepsX) > lastYStep) {
        stepY();
        lastYStep++;
      }
    }
  } else {
    int lastXStep = 0;
    for (int i=1; i<=stepsY; i++) {
      sleepUntilNextMove();
      stepY();
      if (round((double)stepsX * ((double)i - 0.5) / (double)stepsY) > lastXStep) {
        stepX();
        lastXStep++;
      }
    }
  }
  
  _currentX = targetX;
  _currentY = targetY;
}

void GcodePlotter::circularInterpolation(double x, double y, double z, double i, double j, unsigned int feedRate, byte dir) {
  double targetX, targetY, targetZ, centerX, centerY;
  if (_mode == ABSOLUTE_MODE) {
    targetX = x != UNDEF_DOUBLE ? x : _currentX;
    targetY = y != UNDEF_DOUBLE ? y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? z : _currentZ;
  } else {
    targetX = x != UNDEF_DOUBLE ? _currentX + x : _currentX;
    targetY = y != UNDEF_DOUBLE ? _currentY + y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? _currentZ + z : _currentZ;
  }
  // Always relative to start point...
  centerX = i != UNDEF_DOUBLE ? _currentX + i : _currentX;
  centerY = j != UNDEF_DOUBLE ? _currentY + j : _currentY;

  if (feedRate != UNDEF_INTEGER) {
    _feedRate = feedRate; // Set feedrate
  }

  // Z first
  if (targetZ != _currentZ) {
    movePen(targetZ);
  }

  double radius = sqrt((centerX-_currentX)*(centerX-_currentX) + (centerY-_currentY)*(centerY-_currentY));
  double startAngle = calculateAngle(_currentX, _currentY, centerX, centerY);
  double stopAngle = calculateAngle(targetX, targetY, centerX, centerY);

  double angleDiff = 0.0;

  if (dir == CIRC_INTERP_CW) {
    if (stopAngle > startAngle) {
      stopAngle = stopAngle - (2 * M_PI);
    }
    angleDiff = stopAngle - startAngle;
  } else if (dir == CIRC_INTERP_CCW) {
    if (startAngle > stopAngle) {
      stopAngle = stopAngle + (2 * M_PI);
    } 
    angleDiff = stopAngle - startAngle;
  }

  double numIter = radius/2.0;
  double angleDelta = angleDiff / numIter;

  for (int i=1; i < (int) numIter; i++) {
    double newX = centerX + cos(startAngle + i*angleDelta) * radius;
    double newY = centerY + sin(startAngle + i*angleDelta) * radius;
    if (_mode == RELATIVE_MODE) {
      newX = newX - _currentX;
      newY = newY - _currentY;
    }
    linearInterpolation(newX, newY, 0.0, feedRate);
  }
  if (_mode == RELATIVE_MODE) {
    linearInterpolation(targetX - _currentX, targetY - _currentY, 0.0, feedRate);
  } else {
    linearInterpolation(targetX, targetY, 0.0, feedRate);
  }
  
}

// Borrowed algorithm from https://goldberg.berkeley.edu/pubs/XY-Interpolation-Algorithms.pdf
/*
void GcodePlotter::circularInterpolationAlt(double x, double y, double z, double i, double j, byte dir) {
  double targetX, targetY, targetZ, centerX, centerY;
  if (_mode == ABSOLUTE_MODE) {
    targetX = x != UNDEF_DOUBLE ? x : _currentX;
    targetY = y != UNDEF_DOUBLE ? y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? z : _currentZ;
    centerX = i != UNDEF_DOUBLE ? i : _currentX;
    centerY = j != UNDEF_DOUBLE ? j : _currentY;
  } else {
    targetX = x != UNDEF_DOUBLE ? _currentX + x : _currentX;
    targetY = y != UNDEF_DOUBLE ? _currentY + y : _currentY;
    targetZ = z != UNDEF_DOUBLE ? _currentZ + z : _currentZ;
    centerX = i != UNDEF_DOUBLE ? _currentX + i : _currentX;
    centerY = j != UNDEF_DOUBLE ? _currentY + j : _currentY;
  }

  // Z first
  if (targetZ != _currentZ) {
    movePen(targetZ);
  }
  
  int x1 = round(_currentX);
  int y1 = round(_currentY);
  int x2, y2;
  int x3 = round(targetX);
  int y3 = round(targetY);
  int cx = round(centerX);
  int cy = round(centerY);
}
*/

double GcodePlotter::calculateAngle(double x, double y, double cx, double cy) {
  double radius = sqrt((cx-x)*(cx-x) + (cy-y)*(cy-y));
  double cosx = (x-cx)/radius;
  double angle = acos(cosx);
  if (y < cy) {
    angle = (2 * M_PI) - angle;
  }
  return angle;
}

void GcodePlotter::movePen(double z) {
  if (z <= 0.0) {
    _pen->write(_penDownDeg);
    //Serial.println("Pen down");
  } else {
    _pen->write(_penUpDeg);
    //Serial.println("Pen up");
  }
  delay(333);
  _currentZ = z;
}

// Bound within x limits
void GcodePlotter::stepX() {
  _xMotor->isPosDir() ? _currXStep++ : _currXStep--;
  if(_currXStep > 0 && _currXStep < _maxXStep) {
    _xMotor->step();
  }
}

// Bound within y limits
void GcodePlotter::stepY() {
  _yMotor->isPosDir() ? _currYStep++ : _currYStep--;
  if(_currYStep > 0 && _currYStep < _maxYStep) {
    _yMotor->step();
  }
}

unsigned long GcodePlotter::stepRateMicros() {
  //unsigned long stepRateMicros = MICROS_PER_MIN / _feedRate / _stepsPerMm;
  //Serial.print("Step rate us: ");
  //Serial.println(stepRateMicros);
  return MICROS_PER_MIN / _feedRate / _stepsPerMm; 
}

void GcodePlotter::sleepUntilNextMove() {
  boolean doneSleeping = false;
  while (!doneSleeping) {
    if (micros() - _lastMove >= stepRateMicros()) {
      doneSleeping = true;
    }
  }
  _lastMove = micros();
}
