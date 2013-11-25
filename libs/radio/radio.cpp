#include <crypt.h>
#include <radio.h>

static radio_state state;

// initialize library and set device uid
int radio_init(radio_uid uid, uint16_t speed, uint8_t tx_pin, uint8_t rx_pin, uint8_t enable_receiver)
{
  // Serial.print("init");
  state.uid = uid;
  
  vw_set_tx_pin(tx_pin);
  vw_set_rx_pin(rx_pin);
  
  vw_setup(speed);
  
  if( enable_receiver ){
    vw_rx_start();
  }
}

int radio_receive_message(uint8_t *buffer, uint8_t *buflen)
{
  if (vw_get_message(buffer, buflen)) // Non-blocking
  { 
    // decrypt
    xor_data(buffer, *buflen);
    
    if( *buflen >= sizeof(radio_frame_header) ){
      radio_frame_header *header = (radio_frame_header*) buffer;
      return header->msg_type;
    }
    else {
      // ignore invalid packets
      return 0;
    }
  }
  else {
    return 0;
  }
}

int radio_send_state_update(uint8_t sensor_type, uint8_t sensor_index, uint16_t value)
{
  radio_state_update_frame frame;
  uint8_t *buffer = (uint8_t *)&frame;
  memset(&frame, 0, sizeof(frame));
  
  frame.header.source = state.uid;
  frame.header.dest = DEST_MASTER;
  frame.header.msg_type = SENSOR_MSG_STATE_UPDATE;
  
  frame.sensor_type = sensor_type;
  frame.sensor_index = sensor_index;
  frame.value = value;
  
  xor_data(buffer, sizeof(frame));
  
  vw_send(buffer, sizeof(frame));
  vw_wait_tx();
}
