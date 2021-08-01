#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "audio.h"
#include "mod_play.h"

#include "../data/the_softliner.h"

#define SOUND_PIN  15  // sound output
#define LED_PIN    25  // internal LED

#define SOUND_OUTPUT_FREQUENCY 22050 // frequency of the output wave (samples/second)

// Toggle the internal LED every half second
static void blink_led(void)
{
  static uint64_t last_toggle = 0;
  
  uint64_t cur_time = time_us_64();
  if (last_toggle + 500000 < cur_time) {
    gpio_xor_mask(1<<LED_PIN);
    last_toggle = cur_time;
  }
}

// Call the MOD player to fill the output audio buffer.
// This must be called every 20 miliseconds or so, or more
// often if SOUND_OUTPUT_FREQUENCY is increased.
static void update_mod_player(void)
{
  uint8_t *audio_buffer = audio_get_buffer();
  if (audio_buffer) {
    mod_play_step(audio_buffer, AUDIO_BUFFER_SIZE);
  }
}

int main(void)
{
  bi_decl(bi_program_description("MOD test"));
  bi_decl(bi_1pin_with_name(LED_PIN,   "On-board LED"));
  bi_decl(bi_1pin_with_name(SOUND_PIN, "Sound output"));
  
  stdio_init_all();

  printf("==========================\nstarting...\n");
  
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  audio_init(SOUND_PIN, SOUND_OUTPUT_FREQUENCY);
  mod_play_start(&mod_the_softliner, SOUND_OUTPUT_FREQUENCY, 1);
  
  while (1) {
    sleep_ms(20);
    blink_led();
    update_mod_player();
  }
}
