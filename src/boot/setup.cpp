#include "init.h"
#include "app_config.h"
#include <Wire.h>

I2SMicrophone* i2sMicrophone = nullptr;
Notification *notification = nullptr;
Face* faceDisplay = nullptr;

void setupApp(){
	Serial.println("[setupApp] initiate global variable");
	// setupTfWakeWord();

	Wire.begin(SDA_PIN, SCL_PIN);
	
	setupNotification();
	setupEspWakeWord();
	setupMicrophone();
	setupDisplay(SDA_PIN, SCL_PIN);
	setupFaceDisplay(40);
	
}

void setupMicrophone(){
	if (!i2sMicrophone) {
		i2sMicrophone = 
			new I2SMicrophone(MIC_DIN, MIC_SCK, MIC_WS, I2S_NUM_0);
		i2sMicrophone->init(MIC_SAMPLE_RATE, MIC_BIT_PER_SAMPLE);
	}
}

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