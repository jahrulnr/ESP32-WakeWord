#include <Arduino.h>
#include "boot/init.h"
#include "app/tasks.h"

void setup() {
	Serial.begin(115200);
  setCpuFrequencyMhz(240);

  // #if BOARD_HAS_PSRAM
  // heap_caps_malloc_extmem_enable(4096);
  // #endif

  setupApp();
  runTasks();
}

void loop() {
  disableLoopWDT();
  vTaskDelete(NULL);
}