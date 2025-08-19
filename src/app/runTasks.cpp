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

	xTaskCreate(
		loadEspWakeWord,
		"loadEspWakeWordTaskHandle",
		1024 * 8,
		NULL,
		19,
		&displayTaskHandle
	);

	xTaskCreate(
		espWakeWordCaptureTask,
		"espWakeWordCaptureTask",
		1024 * 30,
		NULL,
		0,
		&espWakeWordCaptureTaskHandle
	);

	xTaskCreate(
		displayTask,
		"displayTaskTaskHandle",
		1024 * 8,
		NULL,
		0,
		&displayTaskHandle
	);
}