#include "CommandParser.h"
#include "Arduino.h"

CommandParser::CommandParser() {
}

void CommandParser::wipeCommand(Command* cmd) {
  cmd->cmdType = NO_OP;
  cmd->jointa = UNDEF_INTEGER;
  cmd->jointb = UNDEF_INTEGER;
  cmd->jointc = UNDEF_INTEGER;
  cmd->jointd = UNDEF_INTEGER;
  cmd->jointe = UNDEF_INTEGER;
  cmd->jointf = UNDEF_INTEGER;
}

void CommandParser::parseLine(char* line, Command* cmd) {
  Serial.print("Parsing: ");
  Serial.println(line);
  wipeCommand(cmd);
  parseCommand(line, cmd);
  parseParams(line, cmd);
}

void CommandParser::parseCommand(char* line, Command* cmd) {
  if (strncmp(line, MOVE_TOKEN, 3) == 0) {
    Serial.println("MOVE_TOKEN");
    cmd->cmdType = MOVE;
  } else if (strncmp(line, POSITION_TOKEN, 3) == 0) {
    Serial.println("POSITION_TOKEN");
    cmd->cmdType = POSITION;
  } else if (strncmp(line, LIMITS_TOKEN, 3) == 0) {
    Serial.println("LIMITS_TOKEN");
    cmd->cmdType = LIMITS;
  } else if (strncmp(line, ZERO_TOKEN, 3) == 0) {
    Serial.println("ZERO_TOKEN");
    cmd->cmdType = ZERO;
  }
}

void CommandParser::parseParams(char* line, Command* cmd) {
  char* ptr = strstr(line, JOINTA_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointa = parseIntValue(ptr);
  }
  ptr = strstr(line, JOINTB_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointb = parseIntValue(ptr);
  }
  ptr = strstr(line, JOINTC_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointc = parseIntValue(ptr);
    Serial.print("JC: ");
    Serial.println(cmd->jointc);
  }
  ptr = strstr(line, JOINTD_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointd = parseIntValue(ptr);
  }
  ptr = strstr(line, JOINTE_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointe = parseIntValue(ptr);
  }
  ptr = strstr(line, JOINTF_TOKEN);
  if (ptr) {
    ptr = ptr+2;
    cmd->jointf = parseIntValue(ptr);
  }
}

unsigned int CommandParser::parseIntValue(char* line) {
  char buf[8];
  int i = 0;
  while ((line[i] >= '0' && line[i] <= '9') || line[i] == '-') {
    buf[i] = line[i];
    i++;
  }
  buf[i] = '\0';
  return i>0 ? atoi(buf) : UNDEF_INTEGER;
}


void CommandParser::printCommand(Command cmd) {
  Serial.print("Command: ");
  Serial.print(cmd.cmdType);
  Serial.print(" JA: ");
  Serial.print(cmd.jointa);
  Serial.print(" JB: ");
  Serial.print(cmd.jointb);
  Serial.print(" JC: ");
  Serial.print(cmd.jointc);
  Serial.print(" JD: ");
  Serial.print(cmd.jointd);
  Serial.print(" JE: ");
  Serial.print(cmd.jointe);
  Serial.print(" JF: ");
  Serial.println(cmd.jointf);
}
