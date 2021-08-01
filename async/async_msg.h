#ifndef ASYNC_MSG_H_FILE
#define ASYNC_MSG_H_FILE

#include <stdint.h>
#include <stdbool.h>
#include "pico/util/queue.h"

#define MSG_QUEUE_LENGTH 4  // max messages in queue

#ifdef __cplusplus
extern "C" {
#endif

struct MOD_DATA;

// message types
enum ASYNC_MSG_TYPE {
  ASYNC_MSG_TYPE_AUDIO_INIT,
  ASYNC_MSG_TYPE_MOD_START,
  ASYNC_MSG_TYPE_MOD_STOP,
};

struct ASYNC_MSG_HEADER {
  enum ASYNC_MSG_TYPE type;
};

struct ASYNC_MSG_AUDIO_INIT {
  struct ASYNC_MSG_HEADER msg_header;
  int sound_pin;
  int frequency;
};

struct ASYNC_MSG_MOD_START {
  struct ASYNC_MSG_HEADER msg_header;
  const struct MOD_DATA *mod;
  uint32_t frequency;
  bool loop;
};

struct ASYNC_MSG_MOD_STOP {
  struct ASYNC_MSG_HEADER msg_header;
};

union ASYNC_MSG {
  struct ASYNC_MSG_HEADER msg_header;
  struct ASYNC_MSG_AUDIO_INIT audio_init;
  struct ASYNC_MSG_MOD_START mod_start;
  struct ASYNC_MSG_MOD_STOP mod_stop;
};

extern queue_t async_msg_queue;

static inline bool async_msg_can_recv(void) { return queue_is_empty(&async_msg_queue); }
static inline bool async_msg_can_send(void) { return ! queue_is_full(&async_msg_queue); }

static inline void async_msg_send(union ASYNC_MSG *msg) { return queue_add_blocking(&async_msg_queue, msg); }
static inline void async_msg_recv(union ASYNC_MSG *msg) { return queue_remove_blocking(&async_msg_queue, msg); }

static inline bool async_msg_try_send(union ASYNC_MSG *msg) { return queue_try_add(&async_msg_queue, msg); }
static inline bool async_msg_try_recv(union ASYNC_MSG *msg) { return queue_try_remove(&async_msg_queue, msg); }

void async_init(void);
void async_audio_init(int sound_pin, int frequency);
void async_mod_start(const struct MOD_DATA *mod, int frequency, bool loop);
void async_mod_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* ASYNC_MSG_H_FILE */
