#include "messages.h"
#include "cJSON.h"
#include <string.h>
int beihe_message_apply(beihe_ui_model_t *m, const char *data) {
    if (!m || !data)
        return -1;
    cJSON *root = cJSON_ParseWithOpts(data, NULL, 1), *center;
    int result = -1;
    if (!root) {
        m->malformed++;
        return -1;
    }
    center = cJSON_GetObjectItem(root, "center");
    if (cJSON_IsObject(root) && cJSON_IsString(center) && center->valuestring)
        result = beihe_ui_center(m, center->valuestring, strlen(center->valuestring));
    else
        m->malformed++;
    cJSON_Delete(root);
    return result;
}
