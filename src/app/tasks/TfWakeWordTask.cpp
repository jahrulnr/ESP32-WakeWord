#include "../tasks.h"

// TaskHandle_t tfWakeWordTaskHandle = nullptr;

// void tfWakeWordTask(void* params) {
//   TickType_t lastWakeTime = xTaskGetTickCount();
//   TickType_t updateFrequency = pdMS_TO_TICKS(1);
// 	const char* _tag = "tfWakeWordTask";

// 	while (true)
// 	{
// 		vTaskDelayUntil(&lastWakeTime, updateFrequency);
// 		taskTfWakeWord();
// 	}
// }
