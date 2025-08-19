#pragma once

#include <Arduino.h>
#include "I2SMicrophone.h"
#include "TfWakeWord.h"
#include "EspWakeWord.h"
#include "Display.h"

extern I2SMicrophone* i2sMicrophone;

void setupApp();
void setupMicrophone();