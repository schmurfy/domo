
#include <crypt.h>
#include <radio.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "../../config.h"

void setup()
{
  set_crypt_key(crypt_key);
  power_twi_disable();
  
  Serial.begin(9600);
  radio_init(0x4455, 2000, 4, 2);
}

uint16_t read_light_sensor(int pin)
{
  return analogRead(pin);
}

ISR(WDT_vect){
  
}

void loop()
{
  while(1){
    Serial.println("Sending message...");
    radio_send_state_update(SENSOR_TYPE_TEMP, 0, read_light_sensor(0));
    // delay(2000);
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = (1<< WDP0) | (1 << WDP3); // 8s @ 5v
    WDTCSR |= (1 << WDIE);
    sleep_enable();
    sleep_mode();
    sleep_disable();
  }
}
