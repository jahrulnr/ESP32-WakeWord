#include "app/display_list.h"
#include "Mochi.h"

void displayListening() {
    if (!display) return;
    
    display->clearBuffer();
    display->setFont(u8g2_font_6x10_tf);
    
    // Draw listening indicator
    display->drawStr(10, 20, "Listening...");
    display->drawStr(10, 35, "Say command");
    
    // Draw animated microphone icon
    static uint8_t animate = 0;
    animate = (animate + 1) % 8;
    
    for (int i = 0; i < 3; i++) {
        if (animate > i * 2) {
            display->drawCircle(100 + i * 8, 25, 2);
        }
    }
    
    display->sendBuffer();
}

void displayCommand(const char* command) {
    if (!display) return;
    
    display->clearBuffer();
    display->setFont(u8g2_font_6x10_tf);
    
    // Draw command confirmation
    display->drawStr(10, 20, "Command:");
    display->drawStr(10, 35, command);
    display->drawStr(10, 50, "Executing...");
    
    display->sendBuffer();
    delay(300);
}
