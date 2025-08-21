#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "app_config.h"
#include "constants.h"
#include "app/callback_list.h"
#include "Notification.h"
#include "Display.h"
#include "Face.h"
#include "esp32-hal-sr.h"

#if (MIC_TYPE == MIC_TYPE_I2S)
#include "I2SMicrophone.h"
extern I2SMicrophone* microphone;
void setupI2SMicrophone();
#else 
#include "AnalogMicrophone.h"
extern AnalogMicrophone* amicrophone;
void setupAnalogMicrophone();
#endif

extern Notification* notification;
extern Face* faceDisplay;
extern bool sr_system_running;

void setupApp();

void setupNotification();
void setupFaceDisplay(uint16_t size = 40);
void setupSpeechRecognition();