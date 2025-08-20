#include "app/tasks.h"
#include <esp_log.h>

TaskHandle_t speechRecognitionTaskHandle = nullptr;

void speechRecognitionTask(void* param) {
    const char* TAG = "speechRecognitionTask";
    
    ESP_LOGI(TAG, "Speech Recognition monitoring task started");
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    TickType_t updateFrequency = pdMS_TO_TICKS(1000); // Check every second
    
    // Wait for SR system to be initialized
    while (!sr_system_running) {
        ESP_LOGI(TAG, "Waiting for SR system initialization...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    ESP_LOGI(TAG, "SR system detected, monitoring started");
    
    while (1) {
        vTaskDelayUntil(&lastWakeTime, updateFrequency);
        
        // Monitor system health
        static int counter = 0;
        if (++counter % 30 == 0) {  // Every 30 seconds
            int free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            int internal_heap = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
            ESP_LOGI(TAG, "System Health - Free Heap: %d, Internal: %d", free_heap, internal_heap);
            
            // Check if SR system is still running
            if (sr_system_running) {
                ESP_LOGI(TAG, "SR system running normally");
            } else {
                ESP_LOGW(TAG, "SR system appears to be stopped");
            }
        }
        
        // Handle any notifications that might be relevant to SR
        if (notification && notification->has(NOTIFICATION_COMMAND)) {
            void* event = notification->consume(NOTIFICATION_COMMAND);
            if (event) {
                const char* command = (const char*)event;
                ESP_LOGI(TAG, "Received command notification: %s", command);
                
                // Handle command notifications if needed
                if (strcmp(command, "pause_sr") == 0) {
                    ESP_LOGI(TAG, "Pausing speech recognition");
                    sr_pause();
                } else if (strcmp(command, "resume_sr") == 0) {
                    ESP_LOGI(TAG, "Resuming speech recognition");
                    sr_resume();
                }
            }
				}
        
        taskYIELD(); // Allow other tasks to run
    }
}
