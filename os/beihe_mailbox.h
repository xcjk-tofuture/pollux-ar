#ifndef BEIHE_MAILBOX_H
#define BEIHE_MAILBOX_H
#include <stdint.h>
#include "beihe_board.h"
enum { BEIHE_EVENT_CONNECTION=1,BEIHE_EVENT_MESSAGE=2 };
typedef struct {uint8_t type,state;uint16_t length;char payload[BEIHE_MESSAGE_CAPACITY];} beihe_event_t;
/* Startup creates four copied-message slots. Task-only nonblocking posts.
 * Full mailbox rejects newest event (-1), never overwrites unread data. */
int beihe_mailbox_init(void);
int beihe_mailbox_post(const beihe_event_t *event);
int beihe_mailbox_get(beihe_event_t *event,uint32_t timeout_ms);
#endif
