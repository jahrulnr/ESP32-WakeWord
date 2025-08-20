#pragma once

#include "boot/init.h"
#include "display_list.h"

extern TaskHandle_t displayTaskHandle;
extern TaskHandle_t speechRecognitionTaskHandle;

void runTasks();

void displayTask(void *param);
void speechRecognitionTask(void* param);
