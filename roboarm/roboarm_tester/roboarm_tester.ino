#include <Servo.h>
#include "CommandParser.h"
#include "StepperController.h"

// Servo pinner
#define JC_SRV_PIN 11  // Joint C servo pin
#define JD_SRV_PIN 12
#define JE_SRV_PIN 13
#define JF_SRV_PIN 88 // TO BE DEFINED...

// Endstop/Interrupt
#define END_JA 2
#define END_JB 3

// Stepper pinner
#define MS1_PIN 4
#define MS2_PIN 5
#define ENABLE_PIN 6
#define STEPA_DIR 7
#define STEPA_STEP 8
#define STEPB_DIR 9
#define STEPB_STEP 10

const int BUF_LEN = 64;

Servo _jointC, _jointD, _jointE, _jointF;
StepperController* _jointA;
StepperController* _jointB;

void setup() {
  pinMode(JC_SRV_PIN, OUTPUT);
  pinMode(JD_SRV_PIN, OUTPUT);
  pinMode(JE_SRV_PIN, OUTPUT);
  pinMode(JF_SRV_PIN, OUTPUT);
  
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(STEPA_DIR, OUTPUT);
  pinMode(STEPA_STEP, OUTPUT);
  pinMode(STEPB_DIR, OUTPUT);
  pinMode(STEPB_STEP, OUTPUT);

  pinMode(END_JA, INPUT_PULLUP);
  pinMode(END_JB, INPUT_PULLUP);

  _jointC.attach(JC_SRV_PIN);
  _jointD.attach(JD_SRV_PIN);
  _jointE.attach(JE_SRV_PIN);
  //jf.attach(JF_SRV_PIN);
  
  Serial.begin(9600);
  delay(500);
}

void loop() {
  CommandParser parser;
  Command cmd;
  StepperController jaMotor(STEP_CW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPA_DIR, STEPA_STEP);
  StepperController jbMotor(STEP_CCW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPB_DIR, STEPB_STEP);
  _jointA = &jaMotor;
  _jointB = &jbMotor;

  char line[BUF_LEN];
  char c;
  int lineIndex = 0;

  while(1) {
    
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {
        if ( lineIndex > 0 ) {   
          line[lineIndex] = '\0';
          parser.parseLine(line, &cmd);
        }
        lineIndex = 0;
      } else {
        line[lineIndex++] = c;
      }
    }
    
  }
}

void processCommand(Command cmd) {
  switch (cmd.cmdType) {
    case MOVE:
      moveArm(cmd.jointa, cmd.jointb, cmd.jointc, cmd.jointd, cmd.jointe, cmd.jointf);
      break;
    case ZERO:
      zero();
      break;
    default:
      Serial.println("error: unrecognized command");
  }
}

void moveArm(int ja, int jb, int jc, int jd, int je, int jf) {
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
    _jointC.write(jc);
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

void zero() {
  // TODO: Zero roboarm
  Serial.println("ZERO...");
}
