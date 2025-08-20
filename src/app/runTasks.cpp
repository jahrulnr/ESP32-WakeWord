#include "tasks.h"

void runTasks(){
	// xTaskCreate(
	// 	tfWakeWordTask,
	// 	"tfWakeWordTaskHandle",
	// 	1024 * 30,
	// 	NULL,
	// 	5,
	// 	&tfWakeWordTaskHandle
	// );

	xTaskCreateUniversal(
		loadEspWakeWord,
		"loadEspWakeWordTaskHandle",
		1024 * 8,
		NULL,
		19,
		&displayTaskHandle,
		0
	);

	xTaskCreateUniversal(
		espWakeWordCaptureTask,
		"espWakeWordCaptureTask",
		1024 * 8,
		NULL,
		5,
		&espWakeWordCaptureTaskHandle,
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