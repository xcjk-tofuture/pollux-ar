#include "messages.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
int main(void) {
    beihe_ui_model_t a,b;beihe_ui_init(&a);beihe_ui_init(&b);
    assert(a.connection==BEIHE_BOOT);
    assert(beihe_ui_connection(&a,BEIHE_READY)==0);
    assert(beihe_ui_connection(&a,255)==-1 && a.connection==BEIHE_READY);
    assert(beihe_message_apply(&a,"{\"center\":\"ok\"}")==0 && !strcmp(a.center,"ok"));
    assert(beihe_message_apply(&a,"{}")==-1);
    assert(beihe_message_apply(&a,"{\"center\":12}")==-1);
    assert(beihe_message_apply(&a,"invalid")==-1);
    assert(beihe_message_apply(&a,"{\"center\":\"other\"} garbage")==-1);
    assert(!strcmp(a.center,"ok") && b.center[0]==0);
    char maximum[160];memset(maximum,'x',159);maximum[159]=0;
    assert(beihe_ui_center(&a,maximum,159)==0);
    assert(beihe_ui_center(&a,maximum,160)==-1 && strlen(a.center)==159);
    assert(beihe_message_apply(&a,NULL)==-1);
    assert(beihe_message_apply(NULL,"{}")==-1);
    puts("PASS: AR state range, JSON fields/types/trailing garbage, capacity and model isolation");
    return 0;
}
