// Plotter with limited gcode support
#include "StepperController.h"
#include <Servo.h>
#include "GcodePlotter.h"

// Servo pin
#define SRV_PIN 12

// Endstop/Interrupt
#define END_X 2
#define END_Y 3

// Stepper pinner
#define MS1_PIN 4
#define MS2_PIN 5
#define ENABLE_PIN 6
#define STEPX_DIR 7
#define STEPX_STEP 8
#define STEPY_DIR 9
#define STEPY_STEP 10

#define MAX_X       (double) 140.0  // 140mm
#define MAX_Y       (double) 140.0  // 140mm
#define STEP_PER_MM (double) 10.0    // 10 steps per mm
#define PEN_UP_DEG   70
#define PEN_DOWN_DEG 95

const int BUF_LEN = 128;

Servo servo;

void setup() {
  pinMode(SRV_PIN, OUTPUT);
  
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(STEPX_DIR, OUTPUT);
  pinMode(STEPX_STEP, OUTPUT);
  pinMode(STEPY_DIR, OUTPUT);
  pinMode(STEPY_STEP, OUTPUT);

  pinMode(END_X, INPUT_PULLUP);
  pinMode(END_Y, INPUT_PULLUP);

  servo.attach(SRV_PIN);
  servo.write(PEN_UP_DEG);
  
  Serial.begin(115200);
  delay(500);
}

void loop() {
  StepperController xMotor(STEP_CW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPX_DIR, STEPX_STEP);
  StepperController yMotor(STEP_CCW, HALF_STEP, MS1_PIN, MS2_PIN, ENABLE_PIN, STEPY_DIR, STEPY_STEP);
  GcodePlotter gcodePlotter(MAX_X, MAX_Y, STEP_PER_MM, PEN_UP_DEG, PEN_DOWN_DEG, END_X, END_Y, &servo, &xMotor, &yMotor);

  gcodePlotter.home();

  char line[BUF_LEN];
  char c;
  int lineIndex = 0;
  unsigned long t1, t2;
  
  while(1) {
    t1 = micros();
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {
        if ( lineIndex > 0 ) {   
          t2 = micros();
          Serial.print("Comm: ");
          Serial.println(t2-t1);
          line[lineIndex] = '\0';
          gcodePlotter.processLine(line);
          t1 = micros();
        }
        lineIndex = 0;
      } else {
        line[lineIndex++] = c;
      }
    }
  }
  
}
