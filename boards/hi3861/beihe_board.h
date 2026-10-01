#ifndef BEIHE_BOARD_H
#define BEIHE_BOARD_H
/* Existing deployment values, isolated from business code. Override in board build config. */
#ifndef BEIHE_WIFI_SSID
#define BEIHE_WIFI_SSID "xcjk"
#endif
#ifndef BEIHE_WIFI_PASSWORD
#define BEIHE_WIFI_PASSWORD "66666666"
#endif
#define BEIHE_BROKER_IP "154.44.26.157"
#define BEIHE_BROKER_PORT 1883
#define BEIHE_MQTT_CLIENT "hi3861-AR-01"
#define BEIHE_MQTT_USER "beihe/HI3861_AR"
#define BEIHE_MQTT_PASSWORD "66666666"
#define BEIHE_MQTT_TOPIC "beihe/weChatApp/configData"
#define BEIHE_LED_GPIO 12
/* Original MQTT receive task was disabled. Enable only after TODO acceptance. */
#define BEIHE_ENABLE_MQTT_RECEIVE 0
#define BEIHE_UI_PERIOD_MS 300u
#define BEIHE_MESSAGE_CAPACITY 200u
#endif
