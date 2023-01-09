#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "sound_i2s.h"
#include "mod_play.h"

#include "../data/the_softliner.h"

#define LED_PIN           25
#define SND_PIN_SCL       16
#define SND_PIN_WS        17
#define SND_PIN_SDA       18

#define SOUND_OUTPUT_FREQUENCY 22050

static const struct sound_i2s_config sound_config = {
  .pin_scl         = SND_PIN_SCL,
  .pin_sda         = SND_PIN_SDA,
  .pin_ws          = SND_PIN_WS,
  .sample_rate     = SOUND_OUTPUT_FREQUENCY,
  .bits_per_sample = 8,
  .pio_num         = 0,
};

static void led_init(uint pin)
{
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}

static void led_blink(uint pin)
{
  static uint32_t last_change_ms = 0;
  static uint8_t led_state = 0;
  
  uint32_t cur_ms = to_ms_since_boot(get_absolute_time());
  if (cur_ms - last_change_ms > 500) {
    last_change_ms = cur_ms;
    led_state = ! led_state;
    gpio_put(LED_PIN, led_state);
  }
}

static void update_mod_player(void)
{
  static int8_t *last_buffer;
  static unsigned char tmp_buffer[SOUND_I2S_BUFFER_NUM_SAMPLES];
  
  int8_t *buffer = sound_i2s_get_next_buffer();
  if (buffer != last_buffer) {
    last_buffer = buffer;
    mod_play_step(tmp_buffer, SOUND_I2S_BUFFER_NUM_SAMPLES);
    for (int i = 0; i < SOUND_I2S_BUFFER_NUM_SAMPLES; i++) {
      int8_t sample = tmp_buffer[i] - 128;
      *buffer++ = sample;
      *buffer++ = sample;
    }
  }
}

int main(void)
{
  stdio_init_all();
  led_init(LED_PIN);

  //sleep_ms(3000); printf("=== INIT ==========\n");

  sound_i2s_init(&sound_config);
  mod_play_start(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY, 1);
  sound_i2s_playback_start();

  while (1) {
    led_blink(LED_PIN);
    update_mod_player();
    sleep_ms(10);
  }
}
