
// system libs
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

// my libs
#include <crypt.h>
#include <radio.h>

#include "sensors/dht11/dht11.h"
#include "../../config.h"

char buff[100];
#define debug_print1(X, ARG1) { sprintf(buff, X, ARG1); Serial.println(buff); }
#define debug_print2(X, ARG1, ARG2) { sprintf(buff, X, ARG1, ARG2); Serial.println(buff); }

typedef enum {
  SENSOR_TYPE1 = 0x01, // temp + light
  SENSOR_TYPE2
  
} sensor_board_type_e;

uint8_t sensor_id;
uint8_t sensor_type;

void setup()
{
  int n;
  set_crypt_key(crypt_key);
  power_twi_disable();
  
  Serial.begin(9600);
  radio_init(0x4455, 2000, 4, 2);
  
  // type + id pins
  for(n = 7; n<= 13; n++){
    pinMode(n, INPUT_PULLUP);
  }
  
  sensor_id = read_id();
  sensor_type = read_type();
  
  debug_print1("id: %d", sensor_id);
  debug_print1("type: %d", sensor_type);
}

uint8_t read_type()
{
  uint8_t n;
  uint8_t ret = 0;
  
  for(n = 7; n<= 10; n++){
    if( digitalRead(n) == LOW ){
      bitSet(ret, n - 7);
    }
  }
  
  return ret;
}

uint8_t read_id()
{
  uint8_t n;
  uint8_t ret = 0;
  
  for(n = 11; n<= 13; n++){
    if( digitalRead(n) == LOW ){
      bitSet(ret, n - 11);
    }
  }
  
  return ret;
}

void send_light_sensor()
{
  radio_send_state_update(SENSOR_TYPE_LIGHT, 0, analogRead(0));
}

DHT11 *dht11_sensor = NULL;
void send_temp_sensor()
{
  uint8_t humidity, temp;
  int ret;
  
  if( dht11_sensor == NULL ){
    dht11_sensor = new DHT11(2);
  }
  
  ret = dht11_sensor->read(humidity, temp);
  debug_print1("ret: %d", ret);
  debug_print2("humidity: %u, temp: %u", humidity, temp);
  
  radio_send_state_update(SENSOR_TYPE_TEMP, 0, temp);
  radio_send_state_update(SENSOR_TYPE_HUMIDITY, 0, humidity);
}

// ISR(WDT_vect){
  
// }

void deep_sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1<< WDP0) | (1 << WDP3); // 8s @ 5v
  WDTCSR |= (1 << WDIE);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

void loop()
{
  while(1){
    Serial.println("Sending message...");
    
    if( sensor_type == SENSOR_TYPE1 )
      send_light_sensor();
    
    delay(100);
    
    if( sensor_type == SENSOR_TYPE1 )
      send_temp_sensor();
    
    
    delay(5000);

    // deep_sleep();
  }
}
