#include "Notification.h"

/**
 * @file NotificationExample.cpp
 * @brief Simple FreeRTOS-native notification system examples
 * 
 * This demonstrates the simplified, FreeRTOS-style notification system
 * where you work with void* directly like native FreeRTOS APIs.
 */

// Global notification instance
Notification* notification = nullptr;

/**
 * @brief Example of sending different types of data (FreeRTOS style)
 */
void exampleSendNotifications() {
    if (!notification) return;
    
    // Example 1: Send a pointer to an integer (you manage the memory)
    static int temperature = 25;
    bool success = notification->send("temperature", &temperature);
    ESP_LOGI("Example", "Send temperature: %s", success ? "OK" : "FAILED");
    
    // Example 2: Send a pointer to a float  
    static float humidity = 65.5f;
    success = notification->send("humidity", &humidity);
    ESP_LOGI("Example", "Send humidity: %s", success ? "OK" : "FAILED");
    
    // Example 3: Send a string pointer
    static const char* status = "READY";
    success = notification->send("status", (void*)status);
    ESP_LOGI("Example", "Send status: %s", success ? "OK" : "FAILED");
    
    // Example 4: Send a custom struct pointer
    struct SensorData {
        int id;
        float value;
        uint32_t timestamp;
    };
    
    static SensorData sensor = {1, 23.4f, esp_timer_get_time() / 1000};
    success = notification->send("sensor_data", &sensor);
    ESP_LOGI("Example", "Send sensor data: %s", success ? "OK" : "FAILED");
    
    // Example 5: Send a dynamically allocated buffer
    uint8_t* buffer = (uint8_t*)malloc(100);
    if (buffer) {
        memset(buffer, 0x42, 100);
        success = notification->send("raw_buffer", buffer);
        ESP_LOGI("Example", "Send raw buffer: %s", success ? "OK" : "FAILED");
        // Note: You still own the buffer memory!
    }
}

/**
 * @brief Example of consuming notifications (FreeRTOS style)
 */
void exampleConsumeNotifications() {
    if (!notification) return;
    
    // Example 1: Consume and cast to int*
    void* data = notification->consume("temperature", pdMS_TO_TICKS(1000));
    if (data) {
        int* temp = (int*)data; // Manual casting like FreeRTOS
        ESP_LOGI("Example", "Consumed temperature: %d", *temp);
        // No need to free - we don't own the original static variable
    } else {
        ESP_LOGI("Example", "Temperature notification not found or timeout");
    }
    
    // Example 2: Consume and cast to float*
    data = notification->consume("humidity");
    if (data) {
        float* humid = (float*)data; // Manual casting
        ESP_LOGI("Example", "Consumed humidity: %.1f", *humid);
    }
    
    // Example 3: Consume string
    data = notification->consume("status");
    if (data) {
        const char* status = (const char*)data; // Manual casting
        ESP_LOGI("Example", "Consumed status: %s", status);
    }
    
    // Example 4: Consume custom struct
    data = notification->consume("sensor_data");
    if (data) {
        struct SensorData {
            int id;
            float value;
            uint32_t timestamp;
        };
        SensorData* sensor = (SensorData*)data; // Manual casting
        ESP_LOGI("Example", "Consumed sensor - ID: %d, Value: %.1f, Time: %lu", 
                 sensor->id, sensor->value, sensor->timestamp);
    }
    
    // Example 5: Consume raw buffer
    data = notification->consume("raw_buffer");
    if (data) {
        uint8_t* buffer = (uint8_t*)data; // Manual casting
        ESP_LOGI("Example", "Consumed raw buffer - first byte: 0x%02X", buffer[0]);
        // If this was malloc'd data, you'd free it here
        free(buffer);
    }
}

/**
 * @brief Example of notification management
 */
void exampleNotificationManagement() {
    if (!notification) return;
    
    // Check if notifications exist
    if (notification->has("temperature")) {
        ESP_LOGI("Example", "Temperature notification exists");
    }
    
    // Wait for a specific notification
    ESP_LOGI("Example", "Waiting for 'ready' notification...");
    if (notification->wait("ready", pdMS_TO_TICKS(5000))) {
        ESP_LOGI("Example", "Ready notification received!");
        void* data = notification->consume("ready");
        // Process data...
    } else {
        ESP_LOGI("Example", "Timeout waiting for ready notification");
    }
    
    // Get notification count
    size_t count = notification->count();
    ESP_LOGI("Example", "Pending notifications: %zu", count);
    
    // Remove specific notification
    if (notification->remove("old_notification")) {
        ESP_LOGI("Example", "Removed old notification");
    }
    
    // Clear all notifications
    notification->clear();
    ESP_LOGI("Example", "All notifications cleared");
}

/**
 * @brief Task that produces notifications (FreeRTOS style)
 */
void producerTask(void* param) {
    ESP_LOGI("Producer", "Producer task started");
    
    static int counter = 0;
    static const char* status_active = "ACTIVE";
    static const char* status_idle = "IDLE";
    
    while (true) {
        // Send periodic data
        counter++;
        notification->send("counter", &counter);
        
        // Send system status
        const char* status = (counter % 2 == 0) ? status_active : status_idle;
        notification->send("system_status", (void*)status);
        
        ESP_LOGI("Producer", "Sent notifications - counter: %d, status: %s", 
                 counter, status);
        
        vTaskDelay(pdMS_TO_TICKS(2000)); // Send every 2 seconds
    }
}

/**
 * @brief Task that consumes notifications (FreeRTOS style)
 */
void consumerTask(void* param) {
    ESP_LOGI("Consumer", "Consumer task started");
    
    while (true) {
        // Wait for counter notification
        if (notification->wait("counter", pdMS_TO_TICKS(3000))) {
            void* data = notification->consume("counter");
            if (data) {
                int* counter = (int*)data; // Manual casting
                ESP_LOGI("Consumer", "Received counter: %d", *counter);
            }
        }
        
        // Check for status updates
        void* data = notification->consume("system_status", pdMS_TO_TICKS(100));
        if (data) {
            const char* status = (const char*)data; // Manual casting
            ESP_LOGI("Consumer", "System status: %s", status);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief Initialize and run the notification system example
 */
void initNotificationExample() {
    // Create notification system
    notification = new Notification();
    
    if (!notification) {
        ESP_LOGE("Example", "Failed to create notification system");
        return;
    }
    
    // Run basic examples
    ESP_LOGI("Example", "=== Running basic notification examples ===");
    exampleSendNotifications();
    vTaskDelay(pdMS_TO_TICKS(100));
    exampleConsumeNotifications();
    vTaskDelay(pdMS_TO_TICKS(100));
    exampleNotificationManagement();
    
    // Create producer and consumer tasks for advanced example
    ESP_LOGI("Example", "=== Starting producer/consumer tasks ===");
    xTaskCreate(producerTask, "Producer", 4096, nullptr, 5, nullptr);
    xTaskCreate(consumerTask, "Consumer", 4096, nullptr, 5, nullptr);
}
