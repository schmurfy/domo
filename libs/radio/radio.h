#include <Arduino.h>
#include <VirtualWire.h>

#define DEST_MASTER 0xFFFF

typedef uint16_t radio_uid;

// ----- common
typedef struct {
  radio_uid     uid; // this must be unique on the network
} radio_state;

typedef struct {
  radio_uid   source;
  radio_uid   dest;
  uint8_t     msg_type;
} radio_frame_header;

typedef enum {
  SENSOR_MSG_STATE_UPDATE = 0x01,
} sensor_msg_type_e;

int radio_init(radio_uid uid, uint16_t speed = 2000,
    uint8_t tx_pin = 4,
    uint8_t rx_pin = 8,
    uint8_t enable_receiver = 0
  );

// ----- sensors

// used by sensors to send their current status:
// temp01 = 56°
// temp02 = 45°
typedef struct {
  radio_frame_header  header;
  uint8_t             sensor_type : 5;
  uint8_t             sensor_index : 3;
  uint16_t            value;
} radio_state_update_frame;

typedef enum {
  SENSOR_TYPE_TEMP = 0x01,
  SENSOR_TYPE_LIGHT,
  SENSOR_TYPE_BUTTON
} sensor_type_e;

int radio_send_state_update(uint8_t sensor_type, uint8_t sensor_index, uint16_t value);


// ----- actuators


// ----- central unit
int radio_request_state(uint8_t target_uid);
int radio_require_state_change();


int radio_receive_message(uint8_t *buffer, uint8_t *buflen);
