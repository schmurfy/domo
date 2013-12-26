
#include "serial_comm.h"
#include <string.h>
#include <stdlib.h>


SerialComm::SerialComm(HardwareSerial *s)
{
  this->serial = s;
}

SerialComm::~SerialComm()
{
  
}

// format: <CMD> arg1 arg2\n
int SerialComm::sendMsg(SerialMessage *msg)
{
  uint8_t size, i;
  char *buffer;
  
  // +1 for the space
  size = (uint8_t) strlen(msg->getCmd()) + 1;
  
  for(i = 0; i< msg->argsCount(); i++){
    size += strlen(msg->getArg(i)) + 1;
  }
  
  buffer = (char *)malloc(size + 1);
  strcpy(buffer, msg->getCmd());
  strlcat(buffer, " ", size);
  
  for(i = 0; i< msg->argsCount(); i++){
    strlcat(buffer, msg->getArg(i), size);
    strlcat(buffer, " ", size);
  }
  
  buffer[size - 1] = EOT;
  buffer[size++] = 0x0;
  
  this->serial->write((uint8_t *)buffer, size);
  
  free(buffer);
  return 0;
}

// called when data can be read
void SerialComm::dataAvailable()
{
  char buffer[100];
  size_t len = this->serial->readBytesUntil(0x00, buffer, sizeof(buffer) - 1);
  
  // check if we have our terminator
  if( buffer[len - 1] == EOT ){
    char *p, *tmp;
    p = strchr(buffer, ' ');
    // MSG ARG1 ARG2\x04\x00
    *p = 0x00;
    tmp = p + 1;
    
    SerialMessage *msg = new SerialMessage(buffer);
    
    while( p < (buffer + len) ){
      p = strchr(tmp, ' ');
      if( p != NULL ){
        *p = 0x00;
        msg->addArgument( tmp );
        tmp = p + 1;
      }
      else {
        p = strchr(tmp, 0x04);
        *p = 0x00;
        msg->addArgument( tmp );
        break;
      }
    }
    
    msgReceived(msg);
  }
}


SerialMessage::SerialMessage(const char *_cmd)
{
  this->args_count = 0;
  this->cmd = (char *) malloc( strlen(_cmd) + 1 );
  strcpy(this->cmd, _cmd);
}

SerialMessage::~SerialMessage()
{
  uint8_t i;
  
  for(i = 0; i< this->args_count; i++){
    free(this->args[i]);
  }
  
  free(this->cmd);
}

int SerialMessage::addArgument(const char *s)
{
  uint8_t index = this->args_count++;
  
  this->args[index] = (char *) malloc( strlen(s) + 1 );
  strcpy(this->args[index], s);
  
  return 0;
}

int SerialMessage::addArgument(uint16_t n)
{
  uint8_t index = this->args_count++;
  
  this->args[index] = (char *) malloc(10);
  sprintf(this->args[index], "%x", n);
  
  return 0;
}

int SerialMessage::addArgument(char c, uint16_t n)
{
  uint8_t index = this->args_count++;
  
  this->args[index] = (char *) malloc(10);
  sprintf(this->args[index], "%c:%x", c, n);
  
  return 0;
  
}

const char *SerialMessage::getCmd()
{
  return this->cmd;
}

const char *SerialMessage::getArg(uint8_t n)
{
  return this->args[n];
}

uint8_t SerialMessage::argsCount()
{
  return this->args_count;
}

