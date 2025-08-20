#pragma once

#define SCL_PIN SCL
#define SDA_PIN SDA

enum E_MIC_TYPE {
	MIC_TYPE_I2S,
	MIC_TYPE_ANALOG,
};

#define MIC_TYPE MIC_TYPE_I2S
#ifdef SEED_XIAO_ESP32S3
#define MIC_SCK 42
#define MIC_WS  -1
#define MIC_DIN 41
#else
// #define MIC_SCK 41
// #define MIC_WS  42
// #define MIC_DIN 2
#define MIC_DIN 11 // 2           // I2S data pin (SD)
#define MIC_SCK 5          // I2S clock pin (SCK)
#define MIC_WS 13             // I2S word select pin (WS/FS)
#endif

#define MIC_SAMPLE_RATE 16000
#define MIC_BIT_PER_SAMPLE 32

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64