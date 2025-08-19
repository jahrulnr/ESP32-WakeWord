#include "boot/init.h"
#include "config_mic.h"

I2SMicrophone* i2sMicrophone = nullptr;

void setupMicrophone(){
	if (!i2sMicrophone) {
		i2sMicrophone = 
			new I2SMicrophone(MIC_DIN, MIC_SCK, MIC_WS, I2S_NUM_0);
		i2sMicrophone->init(MIC_SAMPLE_RATE, MIC_BIT_PER_SAMPLE);
	}
}