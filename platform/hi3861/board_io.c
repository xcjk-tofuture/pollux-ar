#include "board_io.h"
#include "beihe_board.h"
#include "iot_gpio.h"
int beihe_board_led_init(void) {
    if (IoTGpioInit(BEIHE_LED_GPIO) != 0)
        return -1;
    return IoTGpioSetDir(BEIHE_LED_GPIO, IOT_GPIO_DIR_OUT) == 0 ? 0 : -1;
}
void beihe_board_led_write(uint8_t on) { IoTGpioSetOutputVal(BEIHE_LED_GPIO, on ? 1 : 0); }
