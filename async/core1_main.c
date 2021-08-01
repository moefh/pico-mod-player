#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "async_msg.h"
#include "../lib/audio.h"
#include "../lib/mod_play.h"

static int playing_mod = 0;

static void audio_step(void)
{
  uint8_t *audio_buffer = audio_get_buffer();
  if (! audio_buffer) return;

  if (playing_mod) {
    // call the mod player to fill the audio buffer
    if (mod_play_step(audio_buffer, AUDIO_BUFFER_SIZE) != 0) {
      playing_mod = 0;
    }
  } else {
    // insert silence into the audio buffer
    memset(audio_buffer, 128, AUDIO_BUFFER_SIZE);
  }
}

static void process_message(union ASYNC_MSG *msg)
{
  //printf("processing message of type %d\n", msg->msg_header.type);
  switch (msg->msg_header.type) {
  case ASYNC_MSG_TYPE_AUDIO_INIT:
    {
      struct ASYNC_MSG_AUDIO_INIT *m = &msg->audio_init;
      audio_init(m->sound_pin, m->frequency);
    }
    return;

  case ASYNC_MSG_TYPE_MOD_START:
    {
      struct ASYNC_MSG_MOD_START *m = &msg->mod_start;
      mod_play_start(m->mod, m->frequency, m->loop);
      playing_mod = 1;
    }
    break;

  case ASYNC_MSG_TYPE_MOD_STOP:
    playing_mod = 0;
    break;

  }

  printf("ERROR processing message: invalid messaged type %d\n", msg->msg_header.type);
}

void core1_main(void)
{
  multicore_fifo_push_blocking(0);
  multicore_fifo_pop_blocking();  // wait until core 0 is ready

  while (true) {
    sleep_ms(20);

    union ASYNC_MSG msg;
    while (async_msg_try_recv(&msg)) {
      process_message(&msg);
    }

    audio_step();
  }
}
