
#include "pico/multicore.h"

#include "async_msg.h"

queue_t async_msg_queue;

void core1_main(void);

void async_init(void)
{
  queue_init(&async_msg_queue, sizeof(union ASYNC_MSG), MSG_QUEUE_LENGTH);

  multicore_launch_core1(core1_main);
  multicore_fifo_pop_blocking();  // wait for core1 to start
  multicore_fifo_push_blocking(0);
}

void async_audio_init(int sound_pin, int frequency)
{
  union ASYNC_MSG msg;
  msg.msg_header.type      = ASYNC_MSG_TYPE_AUDIO_INIT;
  msg.audio_init.sound_pin = sound_pin;
  msg.audio_init.frequency = frequency;
  async_msg_send(&msg);
}

void async_mod_start(const struct MOD_DATA *mod, int frequency, bool loop)
{
  union ASYNC_MSG msg;
  msg.msg_header.type     = ASYNC_MSG_TYPE_MOD_START;
  msg.mod_start.mod       = mod;
  msg.mod_start.frequency = frequency;
  msg.mod_start.loop      = loop;
  async_msg_send(&msg);
}

void async_mod_stop(void)
{
  union ASYNC_MSG msg;
  msg.msg_header.type = ASYNC_MSG_TYPE_MOD_STOP;
  async_msg_send(&msg);
}
