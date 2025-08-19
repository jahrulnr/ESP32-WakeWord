#include "Display.h"
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
int _scl, _sda;
bool _initiate = false;

void setupDisplay(int scl, int sda) {
	_scl = scl;
	_sda = sda;

	Wire.begin(_sda, _scl);
	_initiate = display.begin();
}



