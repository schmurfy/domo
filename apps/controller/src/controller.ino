
#include <crypt.h>
#include <radio.h>
#include <serial_comm.h>

#include "../../config.h"

void setup()
{
  set_crypt_key(crypt_key);
  Serial.begin(9600);
  radio_init(0xFFFF, 2000, 4, 2, 1);
}

void dump_hex(uint8_t *buf, uint8_t buflen)
{
  uint8_t i;
  
  Serial.print("Got: ");
  for (i = 0; i < buflen; i++)
  {
    Serial.print(buf[i], HEX);
    Serial.print(" ");
  }

  Serial.println("");
}

void loop()
{
  uint8_t buffer[30], buflen = 30;
  uint8_t type;
  
  while(1){
    type = radio_receive_message(buffer, &buflen);
    switch(type){
      case SENSOR_MSG_STATE_UPDATE:
        radio_state_update_frame *frame = (radio_state_update_frame *) buffer;
        
        Serial.print("from ");
        Serial.print(frame->header.source, HEX);
        
        Serial.print(" to ");
        Serial.print(frame->header.dest, HEX);
        
        Serial.print(" : ");
        Serial.println(frame->value);
        
        break;
        
      // default:
        // nothing
    }
  }
  
}
