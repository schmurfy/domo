#include <Arduino.h>

#define MAX_ARGS 2

class SerialMessage
{
private:
  char *cmd;
  uint8_t args_count;
  char    *args[MAX_ARGS];

public:
  SerialMessage(const char *_cmd);
  ~SerialMessage();
  int addArgument(const char *s);
  
  const char *getCmd();
  const char *getArg(uint8_t);
  uint8_t argsCount();
};

class SerialComm
{
private:
  HardwareSerial     *serial;

public:
  SerialComm(HardwareSerial *s);
  
  int sendMsg(SerialMessage *msg);
};


// to redefine in user code
// void serial_msg_received(serial_msg *msg)
// {
  
// }