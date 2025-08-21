#include "app/callback_list.h"

#if (MIC_TYPE == MIC_TYPE_ANALOG)
// Analog fill callback for ESP-SR system
esp_err_t sr_analog_fill_callback(void *arg, void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms) {
    // Calculate how many 16-bit samples we need
    int samples_needed = len / sizeof(int16_t);
    int samples_read = 0;
    
    if (amicrophone && amicrophone->isActive()) {
        samples_read = amicrophone->readSamples((int16_t*)out, samples_needed, timeout_ms);
    }
    
    if (samples_read > 0) {
        *bytes_read = samples_read * sizeof(int16_t);
        return ESP_OK;
    }
    
    *bytes_read = 0;
    return ESP_FAIL;
}
#endif