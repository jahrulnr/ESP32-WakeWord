#pragma once

#include <Arduino.h>
#include <driver/i2s.h>

/**
 * I2SMicrophone class for digital microphone input using I2S protocol
 * 
 * Provides audio input capabilities using I2S protocol with PDM or analog microphones.
 */
class I2SMicrophone {
public:
    /**
     * Constructor
     * 
     * @param dataPin Data input pin
     * @param clockPin Clock pin (for PDM microphones) or bit clock pin (for I2S microphones)
     * @param wordSelectPin Word select pin (for I2S microphones, -1 for PDM)
     * @param i2sPort I2S port number (I2S_NUM_0 or I2S_NUM_1)
     * @param isPDM Whether this is a PDM microphone (true) or standard I2S (false)
     */
    I2SMicrophone(int dataPin, int clockPin, int wordSelectPin = -1, 
                 i2s_port_t i2sPort = I2S_NUM_0);

    /**
     * Destructor
     */
    ~I2SMicrophone();

    /**
     * Initialize the I2S microphone
     * 
     * @param sampleRate Sample rate in Hz (8000, 16000, 22050, 44100, 48000)
     * @param bitsPerSample Bits per sample (16 or 32)
     * @return true if initialization was successful, false otherwise
     */
    bool init(uint32_t sampleRate = 16000, int bitsPerSample = 32);

    /**
     * Read audio data from the microphone
     * 
     * @param buffer Buffer to store audio data
     * @param bufferSize Size of the buffer in bytes
     * @param timeoutMs Timeout in milliseconds
     * @return Number of bytes read, or -1 on error
     */
    int readAudioData(uint8_t* buffer, size_t bufferSize, uint32_t timeoutMs = 100);

    /**
     * Read audio samples into an int16_t buffer
     * 
     * @param buffer Buffer to store audio samples (int16_t)
     * @param sampleCount Number of samples to read
     * @param timeoutMs Timeout in milliseconds
     * @return Number of samples read, or -1 on error
     */
    int readSamples(int16_t* buffer, size_t sampleCount, uint32_t timeoutMs = 100);

    /**
     * Read the current audio level (peak value from a short sample)
     * 
     * @return Audio level value (0-32767 for 16-bit samples)
     */
    int readLevel();

    /**
     * Read the peak audio level over a specified duration
     * 
     * @param durationMs Duration to sample in milliseconds
     * @return Peak audio level during the sampling period
     */
    int readPeakLevel(int durationMs = 100);

    /**
     * Read the average audio level over a specified duration
     * 
     * @param durationMs Duration to sample in milliseconds
     * @return Average audio level during the sampling period
     */
    int readAverageLevel(int durationMs = 100);

    /**
     * Check if sound is detected above a threshold
     * 
     * @param threshold The threshold level to compare against
     * @return true if sound level is above threshold, false otherwise
     */
    bool isSoundDetected(int threshold = 2000);

    /**
     * Set the sample rate
     * 
     * @param sampleRate New sample rate in Hz
     * @return true if successful, false otherwise
     */
    bool setSampleRate(uint32_t sampleRate);

    /**
     * Check if the microphone is properly initialized
     * 
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    const char* _tag;
    int _dataPin;
    int _clockPin;
    int _wsPin;
    i2s_port_t _i2sPort;
    bool _initialized;
    uint32_t _sampleRate;
    int _bitsPerSample;
    int _baselineLevel;

    /**
     * Configure I2S interface
     * 
     * @return true if successful, false otherwise
     */
    bool configureI2S();

    /**
     * Get the baseline noise level (for calibration)
     * 
     * @param samplingTime Time to sample baseline in milliseconds
     * @return Baseline noise level
     */
    int calibrateBaseline(int samplingTime = 1000);
};
