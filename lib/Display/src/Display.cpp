#include "Display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display;

void setupDisplay(int sda, int scl) {
	display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, scl, sda);
	display->begin();
}



