/* sound_i2s.h */

#ifndef SOUND_I2S_H_FILE
#define SOUND_I2S_H_FILE

#include <stdint.h>

#define SOUND_I2S_BUFFER_NUM_SAMPLES  1024

struct sound_i2s_config {
  uint8_t  pio_num;
  uint8_t  pin_clock_base;
  uint8_t  pin_data;
  uint16_t sample_rate;
  uint8_t  bits_per_sample;
};

int sound_i2s_init(const struct sound_i2s_config *cfg);
void sound_i2s_playback_start(void);
void *sound_i2s_get_next_buffer(void);
void *sound_i2s_get_buffer(int buffer_num);

extern volatile unsigned int sound_i2s_num_buffers_played;

#endif /* SOUND_I2S_H_FILE */
