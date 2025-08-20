#include "app/display_list.h"

void displaySoundDetector() {
	// Draw header
	display->setFont(u8g2_font_7x13_tf);
	display->drawStr(0, 10, "Sound detector");
	display->drawLine(0, 12, 127, 12);

	// Draw status
	display->setFont(u8g2_font_5x8_tf);
	display->drawStr(0, 25, "Status:");
	display->drawStr(50, 25, "Listen");

	// Draw sound 
	display->drawStr(0, 35, "Mic:");
	int micLevel = i2sMicrophone->readLevel();
	int barWidth = map(micLevel, 0, 4096, 0, 80);
	display->drawFrame(45, 30, 80, 8);
	display->drawBox(45, 30, barWidth, 8);
}