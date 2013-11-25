#include <stdint.h>

static const char *key;

void set_crypt_key(const char *s)
{
  key = s;
}

// data will be modified in-place
int xor_data(uint8_t *data, uint8_t data_len)
{
  uint8_t i, val;
  uint8_t key_pos = 0;
  
  for(i = 0; i< data_len; i++){
    val = data[i];
    data[i] = val ^ key[key_pos++];
  }
  
  return 0;
}


#ifdef TEST

#include <stdio.h>

int main(int argc, char const *argv[])
{
  uint8_t key[] = {0x01, 0x02, 0x03, 0x04};
  uint8_t data[] = {0x55, 0x66, 0x77};
  
  xor_data(key, data, 3);
  printf("%x %x %x\n", data[0], data[1], data[2]);
  
  xor_data(key, data, 3);
  printf("%x %x %x\n", data[0], data[1], data[2]);

  
  return 0;
}

#endif
