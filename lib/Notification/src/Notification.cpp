#include "Notification.h"

const char* Notification::TAG = "Notification";

Notification::Notification() {
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        ESP_LOGE(TAG, "Failed to create mutex");
    }
    ESP_LOGI(TAG, "Notification system initialized");
}

Notification::~Notification() {
    clear();
    if (mutex != nullptr) {
        vSemaphoreDelete(mutex);
    }
    ESP_LOGI(TAG, "Notification system destroyed");
}

bool Notification::send(const char* key, void* data) {
    if (mutex == nullptr || key == nullptr) {
        return false;
    }
    
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex for send");
        return false;
    }
    
    // Remove existing notification with same key if it exists
    auto it = notifications.find(key);
    if (it != notifications.end()) {
        notifications.erase(it);
    }
    
    // Add new notification
    notifications.emplace(key, NotificationItem(data));
    
    ESP_LOGD(TAG, "Notification sent - key: %s, data: %p", key, data);
    
    xSemaphoreGive(mutex);
    return true;
}

void* Notification::consume(const char* key, TickType_t timeout_ticks) {
    if (mutex == nullptr || key == nullptr) {
        return nullptr;
    }
    
    TickType_t start_time = xTaskGetTickCount();
    
    while (true) {
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            auto it = notifications.find(key);
            if (it != notifications.end()) {
                void* data = it->second.data;
                notifications.erase(it);
                
                ESP_LOGD(TAG, "Notification consumed - key: %s, data: %p", key, data);
                
                xSemaphoreGive(mutex);
                return data;
            }
            xSemaphoreGive(mutex);
        }
        
        // Check timeout
        TickType_t elapsed = xTaskGetTickCount() - start_time;
        if (elapsed >= timeout_ticks) {
            ESP_LOGD(TAG, "Timeout waiting for notification: %s", key);
            break;
        }
        
        // Small delay to prevent busy waiting
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    
    return nullptr;
}

bool Notification::has(const char* key) {
    if (mutex == nullptr || key == nullptr) {
        return false;
    }
    
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return false;
    }
    
    bool exists = notifications.find(key) != notifications.end();
    
    xSemaphoreGive(mutex);
    return exists;
}

bool Notification::remove(const char* key) {
    if (mutex == nullptr || key == nullptr) {
        return false;
    }
    
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return false;
    }
    
    auto it = notifications.find(key);
    bool removed = false;
    
    if (it != notifications.end()) {
        ESP_LOGD(TAG, "Removing notification: %s", key);
        notifications.erase(it);
        removed = true;
    }
    
    xSemaphoreGive(mutex);
    return removed;
}

void Notification::clear() {
    if (mutex == nullptr) {
        return;
    }
    
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex for clear");
        return;
    }
    
    size_t count = notifications.size();
    notifications.clear();
    
    ESP_LOGD(TAG, "Cleared %zu notifications", count);
    xSemaphoreGive(mutex);
}

size_t Notification::count() {
    if (mutex == nullptr) {
        return 0;
    }
    
    if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return 0;
    }
    
    size_t count = notifications.size();
    
    xSemaphoreGive(mutex);
    return count;
}

bool Notification::wait(const char* key, TickType_t timeout_ticks) {
    if (key == nullptr) {
        return false;
    }
    
    TickType_t start_time = xTaskGetTickCount();
    
    while (true) {
        if (has(key)) {
            return true;
        }
        
        // Check timeout
        TickType_t elapsed = xTaskGetTickCount() - start_time;
        if (elapsed >= timeout_ticks) {
            ESP_LOGD(TAG, "Timeout waiting for notification: %s", key);
            return false;
        }
        
        // Small delay to prevent busy waiting
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
