#pragma once
#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display;

void setupDisplay(int sda = SDA, int scl = SCL);