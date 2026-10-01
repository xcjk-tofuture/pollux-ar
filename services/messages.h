#ifndef BEIHE_MESSAGES_H
#define BEIHE_MESSAGES_H
#include "ui_model.h"
/* UI task only. JSON payload must be NUL-terminated; validates center:string,
 * object type and bounds. Returns -1 for malformed input; never dereferences a missing key. */
int beihe_message_apply(beihe_ui_model_t *model,const char *json);
#endif
