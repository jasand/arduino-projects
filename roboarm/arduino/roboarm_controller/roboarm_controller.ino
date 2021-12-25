
#include "RoboarmController.h"
#include "CommandParser.h"

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

Servo jc, jd, je, jf;

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

  jc.attach(JC_SRV_PIN);
  jd.attach(JD_SRV_PIN);
  je.attach(JE_SRV_PIN);
  //jf.attach(JF_SRV_PIN);
  
  Serial.begin(9600);
  delay(500);
}

void loop() {
  StepperController jaMotor(STEP_CW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPA_DIR, STEPA_STEP);
  StepperController jbMotor(STEP_CCW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPB_DIR, STEPB_STEP);
  RoboarmController roboarmController(&jaMotor, &jbMotor, &jc, &jd, &je, &jf, END_JA, END_JB);

  char line[BUF_LEN];
  char c;
  int lineIndex = 0;

  while(1) {
    
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {
        if ( lineIndex > 0 ) {   
          line[lineIndex] = '\0';
          roboarmController.processCommand(line);
        }
        lineIndex = 0;
      } else {
        line[lineIndex++] = c;
      }
    }
    
  }
}
