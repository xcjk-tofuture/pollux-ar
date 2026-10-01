#include <stdio.h>
#include <string.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "board_io.h"
#include "wifi_device.h"
#include "hal_bsp_wifi.h"
#include "hal_bsp_mqtt.h"
#include "beihe_display.h"
#include "beihe_board.h"
#include "beihe_mailbox.h"
#include "messages.h"
static void post_state(uint8_t state) {
    beihe_event_t e={0};e.type=BEIHE_EVENT_CONNECTION;e.state=state;
    if(beihe_mailbox_post(&e)!=0) printf("[beihe] state mailbox full\n");
}
static int8_t on_payload(const unsigned char *topic,size_t topic_length,
                         const unsigned char *payload,size_t length) {
    beihe_event_t e={0};
    if(!topic || topic_length!=strlen(BEIHE_MQTT_TOPIC) || memcmp(topic,BEIHE_MQTT_TOPIC,topic_length)!=0) return -1;
    if(!payload || length>=sizeof(e.payload)) return -1;
    e.type=BEIHE_EVENT_MESSAGE;e.length=(uint16_t)length;
    memcpy(e.payload,payload,length);e.payload[length]='\0';
    return (int8_t)beihe_mailbox_post(&e);
}
static void network_task(void *argument) {
    (void)argument;post_state(BEIHE_WIFI);
    if(WiFi_connectHotspots(BEIHE_WIFI_SSID,BEIHE_WIFI_PASSWORD)!=WIFI_SUCCESS) goto fault;
    post_state(BEIHE_TCP);
    if(MQTTClient_connectServer(BEIHE_BROKER_IP,BEIHE_BROKER_PORT)!=0) goto fault;
    post_state(BEIHE_MQTT);
    if(MQTTClient_init(BEIHE_MQTT_CLIENT,BEIHE_MQTT_USER,BEIHE_MQTT_PASSWORD)!=0) goto fault;
    p_MQTTClient_sub_callback=on_payload;
    if(MQTTClient_subscribe(BEIHE_MQTT_TOPIC)!=0) goto fault;
    post_state(BEIHE_READY);
#if BEIHE_ENABLE_MQTT_RECEIVE
    for(;;) {if(MQTTClient_sub()<0) goto fault;osDelay(osKernelGetTickFreq()/5);}
#else
    /* Preserve the disabled receive feature; network reconnection is a dev TODO. */
    osThreadExit();
#endif
    return;
fault:
    MQTTClient_unConnectServer();post_state(BEIHE_FAULT);osThreadExit();
}
static void ui_task(void *argument) {
    beihe_ui_model_t model;beihe_event_t e;uint8_t led=0;
    (void)argument;beihe_ui_init(&model);
    if(beihe_board_led_init()!=0) {printf("[beihe] LED init failed\n");}
    beihe_display_init();
    for(;;) {
        if(beihe_mailbox_get(&e,BEIHE_UI_PERIOD_MS)==0) {
            if(e.type==BEIHE_EVENT_CONNECTION) beihe_ui_connection(&model,e.state);
            else if(e.type==BEIHE_EVENT_MESSAGE) {
                if(beihe_message_apply(&model,e.payload)==0) printf("center: %s\n",model.center);
            }
        } else {
            led^=1u;beihe_board_led_write(led);
            /* Retain the baseline test display; new pages remain in dev TODO. */
            beihe_display_baseline();
        }
    }
}
static void beihe_start(void) {
    const osThreadAttr_t ui={.name="BeiHeUI",.stack_size=4096,.priority=osPriorityAboveNormal};
    const osThreadAttr_t network={.name="BeiHeNetwork",.stack_size=4096,.priority=osPriorityNormal};
    if(beihe_mailbox_init()!=0) {printf("[beihe] mailbox creation failed\n");return;}
    if(osThreadNew(ui_task,NULL,&ui)==NULL) {printf("[beihe] UI creation failed\n");return;}
    if(osThreadNew(network_task,NULL,&network)==NULL) {post_state(BEIHE_FAULT);return;}
}
SYS_RUN(beihe_start);
