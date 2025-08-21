#include "init.h"

#if MIC_TYPE == MIC_TYPE_I2S
I2SMicrophone* microphone = nullptr;
#else
AnalogMicrophone* amicrophone = nullptr;
#endif

Notification *notification = nullptr;
Face* faceDisplay = nullptr;
bool sr_system_running = false;

void setupApp(){
	Serial.println("[setupApp] initiate global variable");

	Wire.begin(SDA_PIN, SCL_PIN);
	
	setupNotification();
#if MIC_TYPE == MIC_TYPE_I2S
	setupI2SMicrophone();
#else
    setupAnalogMicrophone();
#endif
	setupDisplay(SDA_PIN, SCL_PIN);
	setupFaceDisplay(40);
	setupSpeechRecognition();
}

#if MIC_TYPE == MIC_TYPE_I2S
// New setup function for I2SMicrophone using ESP-IDF v5+ API
void setupI2SMicrophone() {
    Serial.println("[setupI2SMicrophone] Initializing I2S Standard driver...");
    
    if (!microphone) {
        microphone = new I2SMicrophone(
            (gpio_num_t)MIC_DIN,    // Data pin
            (gpio_num_t)MIC_SCK,    // Clock pin  
            (gpio_num_t)MIC_WS,     // Word select pin
            I2S_NUM_0               // Port number
        );
        
        // Configure for ESP-SR requirements: 16kHz, 16-bit, mono
        esp_err_t ret = microphone->init(16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
        if (ret != ESP_OK) {
            Serial.printf("[setupI2SMicrophone] ERROR: Failed to initialize I2S Standard driver: %s\n", esp_err_to_name(ret));
            return;
        }
        
        // Start the I2S channel
        ret = microphone->start();
        if (ret != ESP_OK) {
            Serial.printf("[setupI2SMicrophone] ERROR: Failed to start I2S Standard driver: %s\n", esp_err_to_name(ret));
            return;
        }
        
        Serial.println("[setupI2SMicrophone] I2S Standard driver initialized and started successfully");
    }
}
#else
void setupAnalogMicrophone(){
    if (!amicrophone) {
        amicrophone = new AnalogMicrophone(MIC_OUT, MIC_GAIN, MIC_AR);
        
        esp_err_t ret = amicrophone->init();
        if (ret != ESP_OK) {
            Serial.printf("[setupAnalogMicrophone] ERROR: Failed to start analog microphone: %s\n", esp_err_to_name(ret));
            return;
        }
        amicrophone->setGain(INPUT);
        amicrophone->setAttackRelease(true);
        delay(1000);
    }
}
#endif

void setupNotification() {
	if (!notification) {
		notification =
			new Notification();
	}
}

void setupFaceDisplay(uint16_t size) {
	if (!faceDisplay) {
		faceDisplay = new Face(display, SCREEN_WIDTH, SCREEN_HEIGHT, size);
    faceDisplay->Expression.GoTo_Normal();
		faceDisplay->LookFront();

    // Assign a weight to each emotion
    // Normal emotions
    faceDisplay->Behavior.SetEmotion(eEmotions::Normal, 1.0);
    faceDisplay->Behavior.SetEmotion(eEmotions::Unimpressed, 1.0);
    faceDisplay->Behavior.SetEmotion(eEmotions::Focused, 1.0);
    faceDisplay->Behavior.SetEmotion(eEmotions::Skeptic, 1.0);

    // Happy emotions
    faceDisplay->Behavior.SetEmotion(eEmotions::Happy, 1.0);
    faceDisplay->Behavior.SetEmotion(eEmotions::Glee, 1.0);
    faceDisplay->Behavior.SetEmotion(eEmotions::Awe, 1.0);

    // Sad emotions
    faceDisplay->Behavior.SetEmotion(eEmotions::Sad, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Worried, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Sleepy, 0.2);

    // Other emotions
    faceDisplay->Behavior.SetEmotion(eEmotions::Angry, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Annoyed, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Surprised, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Frustrated, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Suspicious, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Squint, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Furious, 0.2);
    faceDisplay->Behavior.SetEmotion(eEmotions::Scared, 0.2);

    faceDisplay->Behavior.Timer.SetIntervalMillis(10000);
		faceDisplay->Blink.Timer.SetIntervalMillis(1000);
    faceDisplay->Look.Timer.SetIntervalMillis(5000);

		faceDisplay->RandomBlink = true;
		faceDisplay->RandomBehavior = 
		faceDisplay->RandomLook = 
			false;

		faceDisplay->Update();
	}
}

void setupSpeechRecognition() {
    void* mic_instance = nullptr;
#if MIC_TYPE == MIC_TYPE_I2S
    if (microphone && microphone->isInitialized()) {
        mic_instance = (void*)microphone;
    } else {
        Serial.println("❌ Cannot setup SR: No active I2S implementation");
        return;
    }
#else
    if (amicrophone && amicrophone->isInitialized()) {
        mic_instance = (void*)amicrophone;
    } else {
        Serial.println("❌ Cannot setup SR: No active Analog implementation");
        return;
    }
#endif
    
    Serial.println("🧠 Setting up Speech Recognition system...");
    
    // Start ESP-SR system with high-level API
    esp_err_t ret = sr_start(
#if MIC_TYPE == MIC_TYPE_I2S
        sr_i2s_fill_callback,                              // I2S data fill callback
#else
        sr_analog_fill_callback,                           // analog data fill callback
#endif
        mic_instance,                                      // Microphone instance (I2SMicrophone or I2SMicrophone)
        SR_CHANNELS_MONO,                                  // Single channel I2S input
        SR_MODE_WAKEWORD,                                  // Start in wake word mode
        voice_commands,                                    // Commands array
        sizeof(voice_commands) / sizeof(sr_cmd_t),         // Number of commands
        sr_event_callback,                                 // Event callback
        NULL                                               // Event callback argument
    );
    
    if (ret == ESP_OK) {
        sr_system_running = true;
        Serial.println("✅ Speech Recognition started successfully!");
        Serial.println("🎯 Say 'Hi ESP' to activate, then try commands:");
        Serial.println("   💡 Light Control:");
        Serial.println("      • 'Turn on the light' / 'Switch on the light'");
        Serial.println("      • 'Turn off the light' / 'Switch off the light' / 'Go dark'");
        Serial.println("   🌀 Fan Control:");
        Serial.println("      • 'Start fan'");
        Serial.println("      • 'Stop fan'");
        Serial.println("");
        Serial.printf("📋 Loaded %d voice commands:\n", sizeof(voice_commands) / sizeof(sr_cmd_t));
        for (int i = 0; i < (sizeof(voice_commands) / sizeof(sr_cmd_t)); i++) {
            Serial.printf("   [%d] Group %d: '%s' -> '%s'\n", 
                        i, 
                        voice_commands[i].command_id,
                        voice_commands[i].str, 
                        voice_commands[i].phoneme);
        }
    } else {
        Serial.printf("❌ Failed to start Speech Recognition: %s\n", esp_err_to_name(ret));
        sr_system_running = false;
    }
}