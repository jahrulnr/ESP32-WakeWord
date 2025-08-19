#pragma once

#include "boot/init.h"

extern TaskHandle_t tfWakeWordTaskHandle;
extern TaskHandle_t espWakeWordTaskHandle;
extern TaskHandle_t espWakeWordCaptureTaskHandle;
extern TaskHandle_t displayTaskHandle;

void runTasks();

void tfWakeWordTask(void* params);
void espWakeWordCaptureTask(void* param);
void displayTask(void *param);
