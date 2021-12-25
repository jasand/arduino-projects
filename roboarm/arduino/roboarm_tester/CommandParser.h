#ifndef CommandParser_h
#define CommandParser_h

#define MOVE     'M'
#define POSITION 'P'
#define LIMITS   'L'
#define ZERO     'Z'
#define NO_OP    '\0'

#define MOVE_TOKEN     "MOV"
#define POSITION_TOKEN "POS"
#define LIMITS_TOKEN   "LIM"
#define ZERO_TOKEN     "ZER"

#define JOINTA_TOKEN  "JA"
#define JOINTB_TOKEN  "JB"
#define JOINTC_TOKEN  "JC"
#define JOINTD_TOKEN  "JD"
#define JOINTE_TOKEN  "JE"
#define JOINTF_TOKEN  "JF"

const unsigned int UNDEF_INTEGER = 6666;

struct Command {
  char cmdType;
  int jointa;
  int jointb;
  int jointc;
  int jointd;
  int jointe;
  int jointf;
};

class CommandParser {
private:
  void wipeCommand(Command* cmd);
  void parseCommand(char* line, Command* cmd);
  void parseParams(char* line, Command* cmd);
  unsigned int parseIntValue(char* line);

public:
  CommandParser();
  void parseLine(char* line, Command* cmd);
  void printCommand(Command cmd);
};

#endif
