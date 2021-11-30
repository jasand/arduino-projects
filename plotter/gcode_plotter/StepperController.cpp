#include "StepperController.h"
#include "Arduino.h"

StepperController::StepperController(byte negativeDir, byte mode, byte ms1Pin, byte ms2Pin, byte enablePin, byte dirPin, byte stepPin) {
  _negativeDir = negativeDir;
  _mode = mode;
  _ms1Pin = ms1Pin;
  _ms2Pin = ms2Pin;
  _enablePin = enablePin;
  _dirPin = dirPin;
  _stepPin = stepPin;
  _direction = STEP_CW;
  setMode(mode);
  enable();
}

void StepperController::setMode(byte mode) {
  _mode = mode;
  // MS1,MS2: L,L=full - H,L=1/2 - L,H=1/4 - H,H=1/8 
  //Serial.print("Setting mode: ");
  //Serial.println(mode);
  switch (_mode) {
    case FULL_STEP:
      digitalWrite(_ms1Pin, LOW);
      digitalWrite(_ms2Pin, LOW);
      break;
    case HALF_STEP:
      digitalWrite(_ms1Pin, HIGH);
      digitalWrite(_ms2Pin, LOW);
      break;
    case QUARTER_STEP:
      digitalWrite(_ms1Pin, LOW);
      digitalWrite(_ms2Pin, HIGH);
      break;
    case ONEEIGHT_STEP:
      digitalWrite(_ms1Pin, HIGH);
      digitalWrite(_ms2Pin, HIGH);
      break;
    default:
      _mode = FULL_STEP;
      digitalWrite(_ms1Pin, LOW);
      digitalWrite(_ms2Pin, LOW);
      break;
  }
}

void StepperController::enable() {
  //Serial.println("enable()");
  digitalWrite(_enablePin, LOW);
}

void StepperController::disable() {
  //Serial.println("disable()");
  digitalWrite(_enablePin, HIGH);
}

void StepperController::setNegDir() {
  _direction = _negativeDir;
  setDirection();
}

void StepperController::setPosDir() {
  if (_negativeDir == STEP_CW) {
    _direction = STEP_CCW;
  } else {
    _direction = STEP_CW;
  }
  setDirection();
}

boolean StepperController::isPosDir() {
  return _direction != _negativeDir;
}

void StepperController::setDirection() {
  if (_direction == STEP_CW) {
    digitalWrite(_dirPin, LOW);
  } else {
    digitalWrite(_dirPin, HIGH);
  }
}

void StepperController::step() {
  digitalWrite(_stepPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_stepPin, LOW);
  delayMicroseconds(10);
}
