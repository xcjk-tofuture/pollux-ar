#include "ui_model.h"
#include <string.h>
void beihe_ui_init(beihe_ui_model_t *m) {memset(m,0,sizeof(*m));}
int beihe_ui_connection(beihe_ui_model_t *m,uint8_t state) {
    if(state>BEIHE_FAULT) return -1;
    m->connection=state;return 0;
}
int beihe_ui_center(beihe_ui_model_t *m,const char *text,size_t length) {
    if(!text || length>=sizeof(m->center)) {m->malformed++;return -1;}
    memcpy(m->center,text,length);m->center[length]='\0';return 0;
}
