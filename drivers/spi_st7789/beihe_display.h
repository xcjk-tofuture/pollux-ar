#ifndef BEIHE_DISPLAY_H
#define BEIHE_DISPLAY_H
/* Only the UI task owns the panel and its SPI bus. Synchronous driver operations. */
void beihe_display_init(void);
void beihe_display_baseline(void);
#endif
