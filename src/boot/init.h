#pragma once

#include <Arduino.h>
#include "constants.h"
#include "Notification.h"
#include "I2SMicrophone.h"
// #include "TfWakeWord.h"
#include "EspWakeWord.h"
#include "Display.h"
#include "Face.h"

extern Notification* notification;
extern I2SMicrophone* i2sMicrophone;
extern Face* faceDisplay;

void setupApp();

void setupNotification();
void setupMicrophone();
void setupFaceDisplay(uint16_t size = 40);