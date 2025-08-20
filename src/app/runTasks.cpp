#include "tasks.h"

void runTasks(){
	xTaskCreateUniversal(
		speechRecognitionTask,
		"speechRecognitionTask",
		1024 * 4,
		NULL,
		8,
		&speechRecognitionTaskHandle,
		0
	);

	xTaskCreateUniversal(
		displayTask,
		"displayTaskTaskHandle",
		1024 * 4,
		NULL,
		19,
		&displayTaskHandle,
		1
	);
}