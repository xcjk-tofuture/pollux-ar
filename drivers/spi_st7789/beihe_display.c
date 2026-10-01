#include "beihe_display.h"
#include "st7789.h"
void beihe_display_init(void) {lcdInit();LCD_Fill(0,0,135,240,WHITE);}
void beihe_display_baseline(void) {LCD_Fill(0,0,100,100,RED);}
