#include "RoboarmController.h"
#include "StepperController.h"
#include "CommandParser.h"

RoboarmController::RoboarmController(StepperController* jA, StepperController* jB, Servo* jC, Servo* jD, Servo* jE, Servo* jF, byte endstopA, byte endstopB) {
  _jointA = jA;
  _jointB = jB;
  _jointC = jC;
  _jointD = jD;
  _jointE = jE;
  _jointF = jF;
  _endstopA = endstopA; 
  _endstopB = endstopB;
}

void RoboarmController::processCommand(char* line) {
  parser.parseLine(line, &cmd);
  parser.printCommand(cmd);
  switch (cmd.cmdType) {
    case MOVE:
      moveArm(cmd.jointa, cmd.jointb, cmd.jointc, cmd.jointd, cmd.jointe, cmd.jointf);
      break;
    case ZERO:
      zero();
      break;
    case POSITION:
      printPosition();
      break;
    case LIMITS:
      printLimits();
      break;
    default:
      Serial.println("error: unrecognized command");
  }
}

void RoboarmController::zero() {
  // TODO: Zero roboarm
  Serial.println("ZERO...");
}

void RoboarmController::moveArm(int ja, int jb, int jc, int jd, int je, int jf) {
  Serial.println("MOVE...");
  if (ja != UNDEF_INTEGER) {
    int steps = ja;
    if(ja < 0) {
      _jointA->setNegDir();
      steps = -steps;
    } else {
      _jointA->setPosDir();
    }
    while (steps > 0) {
       _jointA->step();
       delay(1);
       steps--;
    }
  }
  
  if (jb != UNDEF_INTEGER) {
    int steps = jb;
    if(jb < 0) {
      _jointB->setNegDir();
      steps = -steps;
    } else {
      _jointB->setPosDir();
    }
    while (steps > 0) {
       _jointB->step();
       delay(1);
       steps--;
    }
  }

  if (jc != UNDEF_INTEGER && jc >=0 && jc <= 180) {
    _jointC->write(jc);
    _currJC = jc;
    Serial.print("Joint C: ");
    Serial.println(_currJC);
  }
/*
  if (jd != UNDEF_INTEGER && jd >=0 && jd <= 180) {
    _jointD.write(jd);
    _currJD = jd;
  }

  if (je != UNDEF_INTEGER && je >=0 && je <= 180) {
    _jointE.write(je);
    _currJE = je;
  }

  if (jf != UNDEF_INTEGER && jf >=0 && jf <= 180) {
    _jointF.write(jf);
    _currJF = jf;
  }
*/
}

void RoboarmController::printPosition() {
  Serial.print("POSITION: JA");
  Serial.print(_currJA);
  Serial.print(" JB");
  Serial.print(_currJB);
  Serial.print(" JC");
  Serial.print(_currJC);
  Serial.print(" JD");
  Serial.print(_currJD);
  Serial.print(" JE");
  Serial.print(_currJE);
  Serial.print(" JF");
  Serial.println(_currJF);
}

void RoboarmController::printLimits() {
  Serial.print("LIMITS: JA");
  Serial.print(_lowA);
  Serial.print(":");
  Serial.print(_highA);
  Serial.print(" JB");
  Serial.print(_lowB);
  Serial.print(":");
  Serial.print(_highB);
  Serial.print(" JC");
  Serial.print(_lowC);
  Serial.print(":");
  Serial.print(_highC);
  Serial.print(" JD");
  Serial.print(_lowD);
  Serial.print(":");
  Serial.print(_highD);
  Serial.print(" JE");
  Serial.print(_lowE);
  Serial.print(":");
  Serial.print(_highE);
  Serial.print(" JF");
  Serial.print(_lowF);
  Serial.print(":");
  Serial.println(_highF);
}
