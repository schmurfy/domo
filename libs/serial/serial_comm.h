#include <Arduino.h>

#define MAX_ARGS 2

#define EOT '\x04'

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
  int addArgument(uint16_t n);
  int addArgument(char c, uint16_t n);
  
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
  virtual ~SerialComm();
  
  int sendMsg(SerialMessage *msg);
  void dataAvailable();
  
  virtual void msgReceived(SerialMessage *msg) = 0;
};


// to redefine in user code
// void serial_msg_received(serial_msg *msg)
// {
  
// }
