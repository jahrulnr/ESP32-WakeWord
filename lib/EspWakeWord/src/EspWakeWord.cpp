#include "EspWakeWord.h"
#include <esp_log.h>
#include <esp32-hal-log.h>

bool espWakeWord_initialized = false;
srmodel_list_t *models;
char *model_name;
esp_wn_iface_t *wakenet;
model_iface_data_t *model_data;

int chunks = 0;
int audio_chunksize = 0;
int16_t *buffer = 0;
size_t data_size = 0;
unsigned char* data = NULL;

void setupEspWakeWord(){
	const char* TAG = "EspWakeWord";
	models = esp_srmodel_init("model");

	// Initialize WakeNet
	char *wn_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL);
	if (!wn_name) {
		ESP_LOGE(TAG, "No WakeNet model found");
		esp_srmodel_deinit(models);
    while(1);
	} 

	int wn_index = esp_srmodel_exists(models, wn_name);
	if (wn_index < 0 || wn_index >= models->num) {
		ESP_LOGE(TAG, "WakeNet model %s not found in list", wn_name);
		esp_srmodel_deinit(models);
    while(1);
	} 

	model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, "hiesp");
	wakenet = (esp_wn_iface_t*)esp_wn_handle_from_name(model_name);
	if (!wakenet) {
    Serial.println("ERROR: Failed to get WakeNet handle");
    while(1);
	}
	model_data = wakenet->create(model_name, DET_MODE_95);

	audio_chunksize = wakenet->get_samp_chunksize(model_data) * sizeof(int16_t);
	buffer = (int16_t *) malloc(audio_chunksize);
	
	if (strstr(model_name, "hiesp") != NULL) {
		data = (unsigned char*)hiesp;
		data_size = sizeof(hiesp);
		Serial.printf("wake word: %s, size:%d\n", "hiesp",  data_size);
		espWakeWord_initialized = true;
	}
}

void loadEspWakeWord(void *arg)
{
	if (!espWakeWord_initialized) {
		Serial.println("ESP-WakeWord not initialized");
		vTaskDelete(NULL);
		return;
	}

	while (1) {
		size_t chunk_size = (chunks + 1)*audio_chunksize;
		if (chunk_size <= data_size) {
			memcpy(buffer, data + chunks * audio_chunksize, audio_chunksize);
		} else {
			break;
		}
		
		wakenet_state_t state = wakenet->detect(model_data, buffer);
		if (state == WAKENET_DETECTED) {
			Serial.printf("Detected\n");
		}
		chunks ++;
	}

	// wakenet->destroy(model_data);
	free(buffer);
	vTaskDelete(NULL);
}