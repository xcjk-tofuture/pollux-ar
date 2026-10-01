#ifndef BEIHE_BOARD_IO_H
#define BEIHE_BOARD_IO_H
#include <stdint.h>
/* Task context only. Init checks SDK return values; write is nonblocking. */
int beihe_board_led_init(void);
void beihe_board_led_write(uint8_t on);
#endif
