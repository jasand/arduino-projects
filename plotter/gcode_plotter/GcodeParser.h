#ifndef GcodeParser_h
#define GcodeParser_h

#include "Arduino.h"

#define GCMD 'G'
#define MCMD 'M'
#define XPAR 'X'
#define YPAR 'Y'
#define ZPAR 'Z'
#define IPAR 'I'
#define JPAR 'J'
#define EPAR 'E'
#define FPAR 'F'
#define SPAR 'S'
#define COMMENT ';'

const double UNDEF_DOUBLE = 666.666;
const unsigned int UNDEF_INTEGER = 6666;

struct GcodeCmd {
  char cmdType;
  unsigned int cmdNum;
  double xParam;
  double yParam;
  double zParam;
  double iParam;
  double jParam;
  unsigned int eParam;
  unsigned int fParam;
  double sParam;
};

class GcodeParser {
private:
  void removeEndOfLineComment(char* line);
  void uppercase(char* line);
  void parseCommand(char* line);
  void parseParams(char* line);
  unsigned int parseIntValue(char* line);
  double parseDoubleValue(char* line);
  
protected:
  char _command;
  unsigned int _commandNumber;
  boolean _parseOK;
  double _xval, _yval, _zval, _ival, _jval, _sval;
  unsigned long  _eval, _fval;

public:
  GcodeParser();
  boolean parseLine(char* line, GcodeCmd* cmd);
  void printCommand(GcodeCmd cmd);
};

#endif
