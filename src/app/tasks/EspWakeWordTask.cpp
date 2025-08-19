#include "app/tasks.h"
#include <esp_log.h>

TaskHandle_t espWakeWordCaptureTaskHandle = nullptr;

void espWakeWordCaptureTask(void* param) {
	const char* TAG = "espWakeWordCaptureTask";

  TickType_t lastWakeTime = xTaskGetTickCount();
  TickType_t updateFrequency = pdMS_TO_TICKS(1);

	while(!espWakeWord_initialized || !model_data || !wakenet);

	int chunk_size = wakenet->get_samp_chunksize(model_data);
	Serial.printf("Model Chunk Size = %3d\n", chunk_size);
	int16_t *buffer = (int16_t *)malloc(chunk_size * sizeof(int16_t));
	if (!buffer) {
		ESP_LOGE(TAG, "Buffer allocation failed");
		vTaskDelete(NULL);
		return;
	}

	while (1) {
		vTaskDelayUntil(&lastWakeTime, updateFrequency);

    int bytes_read = i2sMicrophone->readSamples((int16_t *)buffer, chunk_size, 1000);
    if (bytes_read <= 0) continue;
		if (!buffer) {
			continue;
		}

		wakenet_state_t state = wakenet->detect(model_data, buffer);
		// Serial.printf("Sample Audio Read = %3d, State = %3d\n", bytes_read, state);
    if (state == WAKENET_DETECTED || state == 1) {
        Serial.printf("Sample Audio Read = %3d, State = %3d\n", bytes_read, state);
    }
	}
}