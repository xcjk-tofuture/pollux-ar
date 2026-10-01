#include "beihe_mailbox.h"
#include "cmsis_os2.h"
static osMessageQueueId_t mailbox;
int beihe_mailbox_init(void) {
    mailbox = osMessageQueueNew(4, sizeof(beihe_event_t), NULL);
    return mailbox ? 0 : -1;
}
int beihe_mailbox_post(const beihe_event_t *e) {
    return osMessageQueuePut(mailbox, e, 0, 0) == osOK ? 0 : -1;
}
int beihe_mailbox_get(beihe_event_t *e, uint32_t ms) {
    uint32_t ticks = (uint32_t)(((uint64_t)ms * osKernelGetTickFreq() + 999) / 1000);
    return osMessageQueueGet(mailbox, e, NULL, ticks) == osOK ? 0 : -1;
}
