#include <stdint.h>

class HardwareSerial
{
private:
  char *buffer;

public:
  HardwareSerial()
  {
    this->buffer = NULL;
  }
  
  ~HardwareSerial()
  {
    if( this->buffer != NULL ){
      free(this->buffer);
    }
  }
  
  void write(const char *s)
  {
    size_t size = strlen(s) + 1;
    
    if( this->buffer ){
      free(this->buffer);
    }
    
    this->buffer = (char *) malloc(size);
    strlcpy(this->buffer, s, size);
  }
  
  const char *getBuffer()
  {
    return buffer;
  }
};
