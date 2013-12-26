
#include <crypt.h>
#include <radio.h>
#include <serial_comm.h>

#include "../../config.h"



class HostComm : public SerialComm
{
public:
  SerialMessage *last_message;
  
  HostComm(HardwareSerial *s) : SerialComm(s)
  {
    
  }
  
  void msgReceived(SerialMessage *msg)
  {
    Serial.println("received message !");
  }
  
};


HostComm hc(&Serial);

#define EYES_BLUE     9
#define EYES_RED      10
#define EYES_GREEN    11

#define STATUS_RED    12
#define STATUS_GREEN  8
#define STATUS_BLUE   7

void setup()
{
  set_crypt_key(crypt_key);
  Serial.begin(9600);
  radio_init(0xFFFF, 2000, 4, 2, 1);
  
  pinMode(EYES_RED, OUTPUT);
  pinMode(EYES_GREEN, OUTPUT);
  pinMode(EYES_BLUE, OUTPUT);
  
  pinMode(STATUS_RED, OUTPUT);
  pinMode(STATUS_GREEN, OUTPUT);
  pinMode(STATUS_BLUE, OUTPUT);
  
  
  digitalWrite(EYES_RED, HIGH);
  digitalWrite(EYES_BLUE, HIGH);
  digitalWrite(EYES_GREEN, HIGH);
  
  digitalWrite(STATUS_RED, HIGH);
  digitalWrite(STATUS_BLUE, HIGH);
  digitalWrite(STATUS_GREEN, LOW);
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

#define BLINK_SPEED 2000

void blink_led()
{
  static uint8_t led_state = 1;
  static unsigned long last_switch = millis();
  
  unsigned long now = millis();
  
  if( now - last_switch >= (led_state == 0 ? BLINK_SPEED * 2 : BLINK_SPEED) ){
    // sitwh the state and reset last_switch
    last_switch = now;
    // led_on = !led_on;
    led_state = (led_state + 1) % 2;
    
    digitalWrite(STATUS_RED, HIGH);
    digitalWrite(STATUS_BLUE, HIGH);
    digitalWrite(STATUS_GREEN, HIGH);
    
    switch(led_state){
      case 1: digitalWrite(STATUS_GREEN, LOW); break;
      case 2: digitalWrite(STATUS_BLUE, LOW); break;
      case 3: digitalWrite(STATUS_RED, LOW); break;
    }
    
  }
}

void loop()
{
  uint8_t buffer[30], buflen = 30;
  uint8_t msg_type;
  char type;
  
  while(1){
    blink_led();
    
    msg_type = radio_receive_message(buffer, &buflen);
    switch(msg_type){
      case SENSOR_MSG_STATE_UPDATE:
        radio_state_update_frame *frame = (radio_state_update_frame *) buffer;
        
        switch(frame->sensor_type){
          case SENSOR_TYPE_TEMP:      type = 't'; break;
          case SENSOR_TYPE_HUMIDITY:  type = 'h'; break;
          case SENSOR_TYPE_LIGHT:     type = 'l'; break;
          default:
            type = 'X';
        }
        
        SerialMessage msg("STATE_UPDATE");
        msg.addArgument(frame->header.source);
        msg.addArgument(frame->header.dest);
        msg.addArgument(type, frame->value);
        
        hc.sendMsg(&msg);
        
        digitalWrite(EYES_BLUE, LOW);
        delay(100);
        digitalWrite(EYES_BLUE, HIGH);
        
        // Serial.print("from ");
        // Serial.print(frame->header.source, HEX);
        
        // Serial.print(" to ");
        // Serial.print(frame->header.dest, HEX);
        
        // Serial.print(" : ");
        // Serial.println(frame->value);
        
        break;
        
      // default:
        // nothing
    }
  }
  
}
