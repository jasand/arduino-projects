#include "GcodeParser.h"
#include "Arduino.h"

GcodeParser::GcodeParser() {
  _parseOK = false;
  _commandNumber = UNDEF_INTEGER;
  _xval = UNDEF_DOUBLE;
  _yval = UNDEF_DOUBLE;
  _zval = UNDEF_DOUBLE;
  _ival = UNDEF_DOUBLE;
  _jval = UNDEF_DOUBLE;
  _eval = UNDEF_DOUBLE;
  _fval = UNDEF_DOUBLE;
  _sval = UNDEF_DOUBLE;
}

boolean GcodeParser::parseLine(char* line, GcodeCmd* cmd) {
  removeEndOfLineComment(line);
  uppercase(line);
  //Serial.print("Parsing: ");
  //Serial.println(line);
  parseCommand(line);
  parseParams(line);
  boolean check = (_command == GCMD || _command == MCMD) && _commandNumber != UNDEF_INTEGER;
  if (check) {
    cmd->cmdType = _command;
    cmd->cmdNum = _commandNumber;
    cmd->xParam = _xval;
    cmd->yParam = _yval;
    cmd->zParam = _zval;
    cmd->iParam = _ival;
    cmd->jParam = _jval;
    cmd->eParam = _eval;
    cmd->fParam = _fval;
    cmd->sParam = _sval;
  } else {
    Serial.println("error: Command validation error");
  }
  return check;
}

void GcodeParser::removeEndOfLineComment(char* line) {
  for (unsigned int i=0; i < strlen(line); i++) {
    if (line[i] == COMMENT) {
      line[i] = '\0';
    }
  }
}

void GcodeParser::uppercase(char* line) {
  for (unsigned int i=0; i < strlen(line); i++) {
    if (line[i] >= 97 && line[i] <= 122) {
      line[i] = line[i] - 32;
    }
  }
}

void GcodeParser::parseCommand(char* line) {
  _commandNumber = UNDEF_INTEGER;
  char* ptr = strchr(line, GCMD);
  if (ptr == NULL) {
    ptr = strchr(line, MCMD);
  }
  if (ptr != NULL) {
    _command = ptr[0];
    ptr++;
    _commandNumber = parseIntValue(ptr);
  }
}

void GcodeParser::parseParams(char* line) {
  _xval = UNDEF_DOUBLE;
  _yval = UNDEF_DOUBLE;
  _zval = UNDEF_DOUBLE;
  _ival = UNDEF_DOUBLE;
  _jval = UNDEF_DOUBLE;
  _eval = UNDEF_INTEGER;
  _fval = UNDEF_INTEGER;
  _sval = UNDEF_DOUBLE;
  char* ptr = strchr(line, XPAR);
  if (ptr != NULL) {
    ptr++;
    _xval = parseDoubleValue(ptr);
  }
  ptr = strchr(line, YPAR);
  if (ptr != NULL) {
    ptr++;
    _yval = parseDoubleValue(ptr);
  }
  ptr = strchr(line, ZPAR);
  if (ptr != NULL) {
    ptr++;
    _zval = parseDoubleValue(ptr);
  }
  ptr = strchr(line, IPAR);
  if (ptr != NULL) {
    ptr++;
    _ival = parseDoubleValue(ptr);
  }
  ptr = strchr(line, JPAR);
  if (ptr != NULL) {
    ptr++;
    _jval = parseDoubleValue(ptr);
  }
  ptr = strchr(line, EPAR);
  if (ptr != NULL) {
    ptr++;
    _eval = parseIntValue(ptr);
  }
  ptr = strchr(line, FPAR);
  if (ptr != NULL) {
    ptr++;
    _fval = parseIntValue(ptr);
  }
  ptr = strchr(line, SPAR);
  if (ptr != NULL) {
    ptr++;
    _sval = parseDoubleValue(ptr);
  }
}

unsigned int GcodeParser::parseIntValue(char* line) {
  char buf[8];
  int i = 0;
  while (line[i] >= '0' && line[i] <= '9') {
    buf[i] = line[i];
    i++;
  }
  buf[i] = '\0';
  return i>0 ? atoi(buf) : UNDEF_INTEGER;
}

double GcodeParser::parseDoubleValue(char* line) {
  char buf[12];
  int i = 0;
  while ((line[i] >= '0' && line[i] <= '9') || line[i] == '.' || line[i] == '-') {
    buf[i] = line[i];
    i++;
  }
  buf[i] = '\0';
  return i>0 ? atof(buf) : UNDEF_DOUBLE;
}

void GcodeParser::printCommand(GcodeCmd cmd) {
  Serial.print("Command: ");
  Serial.println(cmd.cmdType);
  Serial.print("    Num: ");
  Serial.println(cmd.cmdNum);
  Serial.print("      X: ");
  Serial.print(cmd.xParam);
  Serial.print("  Y: ");
  Serial.print(cmd.yParam);
  Serial.print("  Z: ");
  Serial.print(cmd.zParam);
  Serial.print("  I: ");
  Serial.print(cmd.iParam);
  Serial.print("  J: ");
  Serial.print(cmd.jParam);
  Serial.print("  E: ");
  Serial.print(cmd.eParam);
  Serial.print("  F: ");
  Serial.print(cmd.fParam);
  Serial.print("  S: ");
  Serial.println(cmd.sParam);
}
