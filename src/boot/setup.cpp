#include "init.h"

void setupApp(){
	Serial.println("[setupApp] initiate tasks");
	setupMicrophone();
	// setupTfWakeWord();
	setupEspWakeWord();
	setupDisplay(SCL, SDA);
}