#include "I2SMicrophone.h"
#include <math.h>

I2SMicrophone::I2SMicrophone(int dataPin, int clockPin, int wordSelectPin, 
                           i2s_port_t i2sPort)
    : _dataPin(dataPin), _clockPin(clockPin), _wsPin(wordSelectPin),
      _i2sPort(i2sPort), _initialized(false),
      _sampleRate(16000), _bitsPerSample(32), _baselineLevel(0),
      _tag("I2S Mic") {
}

I2SMicrophone::~I2SMicrophone() {
    if (_initialized) {
        i2s_driver_uninstall(_i2sPort);
        _initialized = false;
    }
}

bool I2SMicrophone::init(uint32_t sampleRate, int bitsPerSample) {
    if (_initialized) {
        return true;  // Already initialized
    }

    _sampleRate = sampleRate;
    _bitsPerSample = bitsPerSample;

    if (!configureI2S()) {
        return false;
    }

    // Calibrate baseline noise level
    _baselineLevel = calibrateBaseline(500);

    _initialized = true;
    return true;
}

bool I2SMicrophone::configureI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = _sampleRate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 3,
        .dma_buf_len = 300,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = -1
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = _clockPin,
        .ws_io_num = _wsPin,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = _dataPin
    };

    // Install I2S driver
    esp_err_t err = i2s_driver_install(_i2sPort, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        Serial.printf("[%s] Failed to install I2S driver: %d\n", _tag, err);
        return false;
    }

    err = i2s_set_pin(_i2sPort, &pin_config);
    if (err != ESP_OK) {
        Serial.printf("[%s] Failed to set I2S pins: %d\n", _tag, err);
        i2s_driver_uninstall(_i2sPort);
        return false;
    }

    err = i2s_zero_dma_buffer(_i2sPort);
    if (err != ESP_OK) {
        Serial.printf("[%s] Error in initializing dma buffer with 0\n", _tag);
    }

    return true;
}

int I2SMicrophone::readAudioData(uint8_t* buffer, size_t bufferSize, uint32_t timeoutMs) {
    if (!_initialized || !buffer || bufferSize == 0) {
        return -1;
    }

    size_t bytesRead = 0;
    esp_err_t err = i2s_read(_i2sPort, buffer, bufferSize, &bytesRead, timeoutMs / portTICK_PERIOD_MS);
    
    if (err != ESP_OK) {
        Serial.printf("[%s] Error reading from I2S: %d\n", _tag, err);
        return -1;
    }

    return bytesRead;
}

int I2SMicrophone::readSamples(int16_t* buffer, size_t sampleCount, uint32_t timeoutMs) {
    if (!_initialized || !buffer || sampleCount == 0) {
        return -1;
    }

    size_t bytesToRead = sampleCount * sizeof(int16_t);
    size_t bytesRead = 0;
    
    esp_err_t err = i2s_read(_i2sPort, buffer, bytesToRead, &bytesRead, timeoutMs / portTICK_PERIOD_MS);
    
    if (err != ESP_OK) {
        Serial.printf("[%s] Error reading from I2S: %d\n", _tag, err);
        return -1;
    }

    return bytesRead / sizeof(int16_t);
}

int I2SMicrophone::readLevel() {
    if (!_initialized) {
        return -1;
    }

    const int samplesPerRead = 300;
    int16_t samples[samplesPerRead];
    
    int samplesRead = readSamples(samples, samplesPerRead);
    if (samplesRead <= 0) {
        return 0;
    }
    
    // Find the peak value
    int16_t peakValue = 0;
    for (int i = 0; i < samplesRead; i++) {
        int16_t absValue = abs(samples[i]);
        peakValue = max(peakValue, absValue);
    }
    
    return peakValue;
}

int I2SMicrophone::readPeakLevel(int durationMs) {
    if (!_initialized || durationMs <= 0) {
        return -1;
    }

    int peakLevel = 0;
    unsigned long startTime = millis();
    
    while (millis() - startTime < durationMs) {
        int currentLevel = readLevel();
        if (currentLevel > peakLevel) {
            peakLevel = currentLevel;
        }
        delay(5); // Small delay to prevent overwhelming the CPU
    }

    return peakLevel;
}

int I2SMicrophone::readAverageLevel(int durationMs) {
    if (!_initialized || durationMs <= 0) {
        return -1;
    }

    long totalLevel = 0;
    int sampleCount = 0;
    unsigned long startTime = millis();
    
    while (millis() - startTime < durationMs) {
        int currentLevel = readLevel();
        if (currentLevel > 0) {
            totalLevel += currentLevel;
            sampleCount++;
        }
        delay(5); // Small delay to prevent overwhelming the CPU
    }

    return sampleCount > 0 ? (int)(totalLevel / sampleCount) : 0;
}

bool I2SMicrophone::isSoundDetected(int threshold) {
    if (!_initialized) {
        return false;
    }

    int currentLevel = readLevel();
    return (currentLevel - _baselineLevel) > threshold;
}

bool I2SMicrophone::setSampleRate(uint32_t sampleRate) {
    if (!_initialized) {
        _sampleRate = sampleRate;
        return true;
    }

    _sampleRate = sampleRate;
    
    // Reconfigure I2S with new sample rate
    i2s_stop(_i2sPort);
    
    esp_err_t err = i2s_set_sample_rates(_i2sPort, sampleRate);
    if (err == ESP_OK) {
        i2s_start(_i2sPort);
        return true;
    }
    
    return false;
}

bool I2SMicrophone::isInitialized() const {
    return _initialized;
}

int I2SMicrophone::calibrateBaseline(int samplingTime) {
    if (samplingTime <= 0) {
        return 0;
    }

    if (!_initialized && _i2sPort == I2S_NUM_0) {
        // Special case - can't calibrate before init, but need some value
        return 500; // Use a reasonable default value
    }

    long totalLevel = 0;
    int sampleCount = 0;
    unsigned long startTime = millis();
    
    // Sample the environment noise for the specified time
    while (millis() - startTime < samplingTime) {
        int level = readLevel();
        if (level > 0) {
            totalLevel += level;
            sampleCount++;
        }
        delay(5); // 5ms delay between samples
    }

    int baseline = sampleCount > 0 ? (int)(totalLevel / sampleCount) : 0;
    
    if (_initialized) {
        _baselineLevel = baseline;
    }
    
    return baseline;
}
