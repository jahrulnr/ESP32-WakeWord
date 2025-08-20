#pragma once

#include <Arduino.h>
#include "constants.h"
#include "app/callback_list.h"
#include "Notification.h"
#include "I2SMicrophone.h"
#include "Display.h"
#include "Face.h"
#include "esp32-hal-sr.h"

extern Notification* notification;
extern I2SMicrophone* microphone;
extern Face* faceDisplay;
extern bool sr_system_running;

void setupApp();

void setupNotification();
void setupI2SMicrophone();
void setupFaceDisplay(uint16_t size = 40);
void setupSpeechRecognition();