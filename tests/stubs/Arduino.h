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
  
  void write(uint8_t *s, size_t size)
  {
    if( this->buffer ){
      free(this->buffer);
    }
    
    this->buffer = (char *) malloc(size);
    strlcpy(this->buffer, (char *)s, size);
  }
  
  const char *getBuffer()
  {
    return buffer;
  }
  
  size_t readBytesUntil( char, char *_buffer, size_t len)
  {
    // just return the buffer
    return strlcpy(_buffer, this->buffer, len);
  }
};
