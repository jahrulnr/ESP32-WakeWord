#pragma once

#include <string>
#include <map>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"

/**
 * @brief A simple, FreeRTOS-native notification system based on key-value pairs
 * 
 * Similar to FreeRTOS queues/notifications but with string keys.
 * Uses void* for data like native FreeRTOS APIs.
 */
class Notification {
private:
    struct NotificationItem {
        void* data;
        TickType_t timestamp;
        
        NotificationItem(void* d) : data(d), timestamp(xTaskGetTickCount()) {}
    };
    
    std::map<std::string, NotificationItem> notifications;
    SemaphoreHandle_t mutex;
    
    static const char* TAG;
    
public:
    /**
     * @brief Constructor - initializes the notification system
     */
    Notification();
    
    /**
     * @brief Destructor - cleans up resources
     */
    ~Notification();
    
    /**
     * @brief Send a notification with void* data (FreeRTOS style)
     * 
     * @param key The notification key
     * @param data Pointer to send (you manage the memory)
     * @return true if sent successfully, false otherwise
     */
    bool send(const char* key, void* data);
    
    /**
     * @brief Consume a notification by key (FreeRTOS style)
     * 
     * @param key The notification key to consume
     * @param timeout_ms Timeout in ticks to wait for notification
     * @return void* pointer to data, or nullptr if not found/timeout
     * @note You need to cast the returned void* to your expected type
     */
    void* consume(const char* key, TickType_t timeout_ticks = pdMS_TO_TICKS(100));
    
    /**
     * @brief Check if a notification exists
     * 
     * @param key The notification key to check
     * @return true if notification exists, false otherwise
     */
    bool has(const char* key);
    
    /**
     * @brief Remove a notification without consuming it
     * 
     * @param key The notification key to remove
     * @return true if removed, false if not found
     */
    bool remove(const char* key);
    
    /**
     * @brief Clear all notifications
     */
    void clear();
    
    /**
     * @brief Get number of pending notifications
     * 
     * @return Number of notifications
     */
    size_t count();
    
    /**
     * @brief Wait for a notification to arrive (blocking)
     * 
     * @param key The notification key to wait for
     * @param timeout_ticks Maximum time to wait in ticks
     * @return true if notification arrived, false if timeout
     */
    bool wait(const char* key, TickType_t timeout_ticks = portMAX_DELAY);
};
