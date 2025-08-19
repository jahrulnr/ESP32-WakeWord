#pragma once

#include <Arduino.h>
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "model_path.h"
#include "Audio/hey_esp.h"

extern bool espWakeWord_initialized;
extern esp_wn_iface_t *wakenet;
extern model_iface_data_t *model_data;

void setupEspWakeWord();
void loadEspWakeWord(void* param);