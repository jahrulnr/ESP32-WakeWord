#include "app/tasks.h"
#include "Mochi.h"

TaskHandle_t displayTaskHandle = nullptr;

void displayTask(void *param) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  TickType_t updateFrequency = pdMS_TO_TICKS(33);
	size_t updateDelay = 0;
	const char* lastEvent;

	// wait notification initiate
	while (!notification)
		taskYIELD();
	

	while(1) {
		vTaskDelayUntil(&lastWakeTime, updateFrequency);
		display->clearBuffer();

		if (!notification->has(NOTIFICATION_DISPLAY) && updateDelay == 0) {
			displaySoundDetector();
	    display->sendBuffer();
			continue;
		} 

		void* event = notification->has(NOTIFICATION_DISPLAY) 
			? notification->consume(NOTIFICATION_DISPLAY, updateFrequency)
			: nullptr;
		if ((event && strcmp((const char*)event, EVENT_DISPLAY_WAKEWORD) == 0) || strcmp(lastEvent, EVENT_DISPLAY_WAKEWORD) == 0) {
			if (updateDelay == 0) {
				updateDelay = millis() + 3000;
				lastEvent = EVENT_DISPLAY_WAKEWORD;
			}
			// displayHappyFace();
			Mochi::drawFrame(display);
			// send buffer will handled by Face class
		}

		if (updateDelay <= millis()) {
			updateDelay = 0;
			lastEvent = "";
		}
	}
}