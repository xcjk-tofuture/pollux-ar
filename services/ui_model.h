#ifndef BEIHE_UI_MODEL_H
#define BEIHE_UI_MODEL_H
#include <stdint.h>
#include <stddef.h>
enum {
    BEIHE_BOOT = 0,
    BEIHE_WIFI = 1,
    BEIHE_TCP = 2,
    BEIHE_MQTT = 3,
    BEIHE_READY = 4,
    BEIHE_FAULT = 5
};
typedef struct {
    uint8_t connection;
    char center[160];
    uint32_t malformed, dropped;
} beihe_ui_model_t;
void beihe_ui_init(beihe_ui_model_t *model);
/* Nonblocking pure model update, returns -1 for out-of-range state or oversized text. */
int beihe_ui_connection(beihe_ui_model_t *model, uint8_t state);
int beihe_ui_center(beihe_ui_model_t *model, const char *text, size_t length);
#endif
