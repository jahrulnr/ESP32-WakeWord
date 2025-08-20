# ESP-SR Model Management Guide

This guide covers building, packaging, and flashing ESP-SR models for wake word detection and speech recognition.

> **Source Reference**: This documentation incorporates insights from [Xu Jiwei's comprehensive ESP32-S3 + ESP-SR + ESP-TTS tutorial](https://xujiwei.com/blog/2025/04/esp32-arduino-esp-sr-tts/) which provides excellent troubleshooting guidance for Arduino framework integration with ESP-SR.

## 🚀 Quick Start

### Build and Flash (Matches your platformio.ini)
```bash
# 1. Copy models that match your build configuration
cp -r wakenet_model/wn9s_hiesp target/
cp -r vadnet_model/vadnet1_medium target/

# 2. Pack models into binary
python3 pack_model.py -m target -o srmodels.bin

# 3. Flash to your ESP32-S3-DevKitC-1-N16R8 (hiesp.csv partition table)
esptool.py --baud 2000000 write_flash 0x710000 srmodels.bin
```

### Build Project
```bash
# ESP32-S3-DevKitC-1-N16R8
pio run -e esp32-s3-devkitc1-n16r8
```

## 📁 Model Structure

```
model/
├── multinet_model/         # MultiNet speech command recognition models
│   ├── fst/               # Finite State Transducer files
│   ├── mn3_cn/            # MultiNet v3 Chinese
│   ├── mn4_cn/            # MultiNet v4 Chinese
│   ├── mn5q8_cn/          # MultiNet v5 quantized Chinese
│   ├── mn5q8_en/          # MultiNet v5 quantized English
│   ├── mn6_cn/            # MultiNet v6 Chinese
│   ├── mn6_en/            # MultiNet v6 English
│   └── mn7_cn/            # MultiNet v7 Chinese
├── wakenet_model/         # WakeNet wake word detection models
│   ├── wn8_hiesp/         # "Hi ESP" wake word (WakeNet v8)
│   ├── wn9_hiesp/         # "Hi ESP" wake word (WakeNet v9)
│   ├── wn9s_hiesp/        # "Hi ESP" wake word (WakeNet v9 Small)
│   ├── wn9_alexa/         # "Alexa" wake word
│   └── [other wake words] # Various wake word models
├── vadnet_model/          # Voice Activity Detection models
│   └── vadnet1_medium/    # VAD model for voice detection
├── nsnet_model/           # Noise Suppression models
│   ├── nsnet1/            # NS model v1
│   └── nsnet2/            # NS model v2
└── target/                # Built models output
    ├── srmodels.bin       # Packed model binary
    ├── wn9_hiesp/         # Target wake word model
    ├── mn6_en_ctc/        # Target speech recognition model
    └── fst/               # Target FST files
```

## 🔧 Building Models

### Prerequisites
- Python 3.x
- ESP-IDF or ESP-SR framework

### Available Models

#### Wake Word Models (WakeNet)
- `wn8_hiesp` - "Hi ESP" (WakeNet v8)
- `wn9_hiesp` - "Hi ESP" (WakeNet v9, recommended)
- `wn9s_hiesp` - "Hi ESP" (WakeNet v9 Small, lower memory)
- `wn9_alexa` - "Alexa"
- `wn9_xiaoaitongxue` - "小爱同学" (Chinese)

#### Speech Recognition Models (MultiNet)
- `mn6_en` - English commands (MultiNet v6)
- `mn6_cn` - Chinese commands (MultiNet v6)
- `mn7_cn` - Chinese commands (MultiNet v7, latest)

#### Voice Processing Models
- `vadnet1_medium` - Voice Activity Detection
- `nsnet1/nsnet2` - Noise Suppression

### Build Custom Target

1. **Create target directory structure:**
```bash
cd model
mkdir -p target/your_target_name
```

2. **Copy required models to target:**
```bash
# Example: Hi ESP wake word + English commands + VAD
cp -r wakenet_model/wn9_hiesp target/
cp -r multinet_model/mn6_en target/
cp -r vadnet_model/vadnet1_medium target/
cp -r multinet_model/fst target/
```

3. **Pack models into binary:**
```python
# Pack all models in target directory
python3 pack_model.py -m target -o srmodels.bin

# Or pack specific model directory
python3 pack_model.py -m wakenet_model/wn9_hiesp -o hiesp_only.bin
```

### Pre-built Configurations

#### Configuration 1: Hi ESP + English Commands
```bash
python3 pack_model.py -m target -o srmodels.bin
```
Contains: `wn9_hiesp`, `mn6_en_ctc`, `vadnet1_medium`, `fst`

#### Configuration 2: Chinese Wake Words + Commands
```bash
# "小鸭小鸭" (Xiao Ya Xiao Ya) + Chinese commands
cp -r wakenet_model/wn7_xiaoaitongxue target/
cp -r multinet_model/mn6_cn target/
cp -r vadnet_model/vadnet1_medium target/
cp -r multinet_model/fst target/
python3 pack_model.py -m target -o xiaoya_cn.bin
```

#### Configuration 3: "小爱同学" (Xiao Ai Tong Xue)
```bash
cp -r wakenet_model/wn7_xiaoaitongxue target/
cp -r multinet_model/mn6_cn target/
python3 pack_model.py -m target -o xiaoai.bin
```

#### Configuration 4: Hi ESP Small (wn9s_hiesp) - Recommended
```bash
# This matches the platformio.ini CONFIG_SR_WN_WN9S_HIESP=y setting
cp -r wakenet_model/wn9s_hiesp target/
cp -r vadnet_model/vadnet1_medium target/
python3 pack_model.py -m target -o hiesp_small.bin
```
**Features**: Optimized small model, faster processing, lower memory usage

#### Configuration 5: Minimal Hi ESP Only
```bash
python3 pack_model.py -m wakenet_model/wn9_hiesp -o hiesp_minimal.bin
```

#### Configuration 3: Chinese Wake Word + Commands
```bash
# Create target with Chinese models
mkdir -p target_cn
cp -r wakenet_model/wn9_xiaoaitongxue target_cn/
cp -r multinet_model/mn7_cn target_cn/
cp -r vadnet_model/vadnet1_medium target_cn/
python3 pack_model.py -m target_cn -o srmodels_cn.bin
```

## 📱 Flashing Models

### Current Partition Layout (16MB ESP32-S3)
```
model partition: 0x710000 (7.8MB)
voice_data:     0xCF0000 (3MB) 
```

### Flash Model Binary
```bash
# Flash main SR models to model partition
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 2000000 \
    --before default_reset --after hard_reset \
    write_flash 0x710000 target/srmodels.bin

# Verify flash
esptool.py --chip esp32s3 --port /dev/ttyUSB0 \
    verify_flash 0x710000 target/srmodels.bin
```

### Flash TTS Voice Data (Optional)
```bash
# Flash TTS voice data (if available)
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 2000000 \
    --before default_reset --after hard_reset \
    write_flash 0xCF0000 data/esp_tts_voice_data_xiaoxin_small.dat
```

### Legacy 8MB Partition Addresses
```bash
# For 8MB ESP32 partition layout
esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 2000000 \
    write_flash 0x710000 target/srmodels.bin
```

## 🛠️ Development Commands

### Board Configuration
Your platformio.ini supports two board configurations:

#### ESP32-S3-DevKitC-1-N16R8 (Primary)
```bash
pio run -e esp32-s3-devkitc1-n16r8
```
- **Flash**: 16MB
- **PSRAM**: 8MB  
- **Model Partition**: 0x710000 (7.8MB available)

### List Available Models
```bash
find . -name "_MODEL_INFO_" -exec dirname {} \; | sort
```

### Check Model Sizes
```bash
# Check individual model sizes
du -sh wakenet_model/*
du -sh multinet_model/*

# Check built binary size
ls -lh target/srmodels.bin
```

### Model Information
```bash
# View model info files
cat wakenet_model/wn9_hiesp/_MODEL_INFO_
cat multinet_model/mn6_en/_MODEL_INFO_
```

### Debug Model Loading
```bash
# Enable verbose output during packing
python3 pack_model.py -m target -o srmodels.bin --verbose
```

## 📋 Model Selection Guide

### For Wake Word Detection Only
- **Recommended**: `wn9_hiesp` (best accuracy)
- **Low Memory**: `wn9s_hiesp` (smaller footprint)
- **Legacy**: `wn8_hiesp` (older version)

### For Wake Word + Commands
- **English**: `wn9_hiesp` + `mn6_en` + `vadnet1_medium`
- **Chinese**: `wn9_xiaoaitongxue` + `mn7_cn` + `vadnet1_medium`
- **Bilingual**: Include both language models

### Memory Considerations
- **8MB Flash**: Use minimal models (`wn9s_hiesp` only)
- **16MB Flash**: Full model suite with TTS support
- **PSRAM Required**: For mn7+ models and complex processing

## 🔍 Troubleshooting

### Common Issues
1. **Model not found**: Check model paths and _MODEL_INFO_ files
2. **Flash size exceeded**: Use smaller models or increase partition size
3. **Loading failed**: Verify partition addresses match your board configuration

### Debugging
```bash
# Check partition layout
esptool.py --port /dev/ttyUSB0 read_flash 0x8000 0x1000 partition_table.bin
python3 -m esptool partition_table partition_table.bin
```

## 🔧 Troubleshooting Common Issues

### "AFE_SR: ERROR: Please select wake words!"
This error typically occurs when:
1. **Models not flashed**: Ensure models are properly flashed to the model partition
2. **Wrong wake word name**: The ESP_SR.cpp may need modification to match your wake word
3. **Partition mismatch**: Verify the model partition address matches your partition table

**Solution**: Modify the wake word lookup in ESP_SR.cpp:
```cpp
// For "xiaoaitongxue" wake word:
afe_config.wakenet_model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, "xiaoaitongxue");
// or directly:
afe_config.wakenet_model_name = (char*)"wn9_xiaoaitongxue";
```

### I2S Initialization Failure on ESP32-S3 with PSRAM
If you see I2S setup errors, this is often due to interrupt functions being placed in PSRAM instead of SRAM. This was a known issue with certain arduino-esp32 versions.

**Solution**: Use the updated arduino-esp32 framework (3.2.0-RC2 or later) from:
```ini
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
```

### I2S Driver Compatibility (ESP-IDF v5+)
**Critical**: Your `I2SMicrophone.cpp` uses the ESP-IDF native driver, which maintains compatibility across ESP-IDF versions.

**ESP-IDF Native I2S Driver (your implementation):**
```cpp
// ESP-IDF native driver - works in both v4.x and v5.x
#include "driver/i2s.h"
i2s_driver_install(_i2sPort, &i2s_config, 0, NULL);
i2s_read(_i2sPort, buffer, bytesToRead, &bytesRead, timeout);
```

**Your Implementation**: Your `I2SMicrophone.cpp` correctly uses the ESP-IDF native driver, which maintains compatibility across ESP-IDF versions.

**Compatibility**: Your ESP-IDF native driver code works with both current and future ESP-IDF versions.

**Workaround**: Your `I2SMicrophone.cpp` implementation uses the correct ESP-IDF native driver approach.

### ESP-SR + I2S Microphone Integration
ESP-SR requires careful I2S configuration when using external microphones:

**Your Implementation Advantage:**
Your `I2SMicrophone.cpp` uses ESP-IDF native driver, making it compatible with both current and future ESP-IDF versions.

**Compatible Platform Configuration:**
```ini
; Use ESP-IDF v4.x compatible version for legacy I2S support
platform = https://github.com/pioarduino/platform-espressif32/archive/refs/tags/55.03.30-2.tar.gz
framework = arduino

; I2S-specific build flags
build_flags = 
    ${env.build_flags}
    -DCONFIG_I2S_ENABLE_DEBUG_LOG=n
    -DCONFIG_I2S_ISR_IRAM_SAFE=y
```

**I2S + ESP-SR Sample Rate Matching:**
- ESP-SR expects 16kHz sample rate
- I2S microphone must be configured for 16kHz
- Ensure I2S buffer size matches ESP-SR frame requirements

**Testing I2S Compatibility:**
```cpp
// In setup(), test I2S before initializing ESP-SR
if (!microphone.init(16000, 16)) {
    Serial.println("I2S microphone init failed - check driver compatibility");
    return;
}
```

### Missing ESP-SR Libraries
PlatformIO's default arduino-esp32 package excludes ESP-SR libraries.

**Solution**: Use the complete arduino-esp32 build in platformio.ini:
```ini
; Use pioarduino platform for complete ESP-SR support
platform = https://github.com/pioarduino/platform-espressif32/archive/refs/tags/55.03.30-2.tar.gz
framework = arduino

; Essential ESP-SR build flags
build_flags = 
    -DCORE_DEBUG_LEVEL=3
    -mfix-esp32-psram-cache-issue
    -mfix-esp32-psram-cache-strategy=memw
    -DCONFIG_SR_VADN_VADNET1_MEDIUM=y
    -DCONFIG_SR_WN_WN9S_HIESP=y
    -DCONFIG_ESP32S3_INSTRUCTION_CACHE_32KB=y
    -DCONFIG_ESP32S3_DATA_CACHE_64KB=y
    -DCONFIG_ESP32S3_DATA_CACHE_LINE_64B=y

; Use custom partition table
board_build.partitions = hiesp.csv
```

### Platform Configuration Notes
- **ESP-IDF Version**: Uses v4.x compatible version for legacy I2S driver support
- **PSRAM Cache Fixes**: The `-mfix-esp32-psram-cache-issue` flags are crucial for ESP32-S3 with PSRAM
- **SR Model Selection**: `CONFIG_SR_WN_WN9S_HIESP=y` enables the "Hi ESP" small wake word model
- **VAD Configuration**: `CONFIG_SR_VADN_VADNET1_MEDIUM=y` enables voice activity detection
- **Cache Optimization**: ESP32-S3 cache settings optimize performance for speech processing
- **I2S Compatibility**: Platform version ensures legacy I2S driver support for microphone integration

### TTS Audio Quality Issues
If TTS output sounds distorted:
- **Mono vs Stereo**: ESP-TTS outputs mono audio, configure I2S for mono mode
- **Sample Rate**: Ensure 16kHz sample rate for ESP-TTS compatibility
- **Bit Depth**: Use 16-bit audio format
- **I2S Configuration**: Use ESP-IDF native driver for audio output setup

---
*Last updated: August 2025*
```python
python3 pack_model.py -m target -o srmodels.bin
```

# flash to model partition (your hiesp.csv partition table)
```shell
esptool.py --baud 2000000 --before default_reset --after hard_reset write_flash 0x710000 srmodels.bin
```

# flash tts (if using TTS voice data)
```shell
esptool.py --baud 2000000 --before default_reset --after hard_reset write_flash 0xD10000 esp_tts_voice_data_xiaoxin_small.dat
```

---

# References:
- https://xujiwei.com/blog/2025/04/esp32-arduino-esp-sr-tts/
- https://github.com/espressif/esp-sr