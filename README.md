# ESP32 Wake Word Detection with ESP-SR

This project implements wake word detection and speech recognition on ESP32-S3 using Espressif's ESP-SR framework with PlatformIO and Arduino framework.

## 🚀 Quick Start

### Prerequisites
- PlatformIO Core 6.1+
- **ESP32-S3-DevKitC-1-N16R8** (16MB Flash, 8MB PSRAM) - **Primary tested board**
- Python 3.x for model building
- esptool.py for manual model flashing

**Note**: This project is specifically tested and configured for ESP32-S3-DevKitC-1-N16R8. Other ESP32 boards may require adjustments to the partition table (`hiesp.csv`) based on their flash memory size and layout.

### Build and Flash
```bash
# 1. Build project for ESP32-S3-DevKitC-1-N16R8
pio run -e esp32-s3-devkitc1-n16r8

# 2. Upload firmware
pio run -e esp32-s3-devkitc1-n16r8 -t upload

# 3. Build and flash models (see Model Management section below)
cd model
python3 pack_model.py -m target -o srmodels.bin
esptool.py --baud 2000000 write_flash 0x710000 srmodels.bin
```

## 📋 Project Structure

```
WakeWord/
├── src/                        # Main application code
│   ├── main.cpp               # Application entry point
│   ├── app/                   # Application logic
│   └── boot/                  # Boot sequence
├── include/                   # Project header files
│   └── config_mic.h           # I2S microphone pin configuration
├── lib/                       # Custom libraries
│   ├── EspWakeWord/           # ESP-SR wake word wrapper
│   ├── Microphone/            # I2S microphone interface
│   ├── Display/               # U8g2 display support
│   └── AppModels/             # Application data models
├── model/                     # ESP-SR model management
│   ├── pack_model.py          # Model packing script
│   ├── movemodel.py           # ESP-IDF model mover
│   ├── target/                # Built model output
│   ├── wakenet_model/         # Wake word detection models
│   ├── multinet_model/        # Speech command recognition
│   ├── vadnet_model/          # Voice activity detection
│   └── nsnet_model/           # Noise suppression
├── esp-sr/                    # ESP-SR framework submodule
├── platformio.ini             # PlatformIO configuration
├── hiesp.csv                  # Custom partition table
└── boards/                    # Custom board definitions
```

## 🔧 Hardware Configuration

### Supported Boards

#### ESP32-S3-DevKitC-1-N16R8 (Primary)
- **Flash**: 16MB
- **PSRAM**: 8MB
- **Model Partition**: 0x710000 (5.9MB available)
- **Build Environment**: `esp32-s3-devkitc1-n16r8`

### Partition Layout (hiesp.csv)
```
Name      Type    SubType   Offset    Size      Purpose
app0      app     ota_0     0x10000   3MB       Primary firmware
app1      app     ota_1     0x310000  3MB       OTA update firmware  
spiffs    data    spiffs    0x610000  1MB       File system
model     data    spiffs    0x710000  5.9MB     ESP-SR models
voice_data data   fat       0xCF0000  3MB       TTS voice data
```

## 🎯 Wake Word Models

### Current Configuration
- **Wake Word**: "Hi ESP" (wn9s_hiesp)
- **Model Type**: WakeNet 9 Small (optimized)
- **Language**: English
- **Detection Mode**: Normal (DET_MODE_90)

### Available Wake Words
```
wn9s_hiesp      - "Hi ESP" (Small, recommended)
wn9_hiesp       - "Hi ESP" (Full model)  
wn9_alexa       - "Alexa"
wn9_xiaoaitongxue - "小爱同学" (Chinese)
wn9_hilexin     - "Hi Lexin"
```

## 📦 Model Management

**For detailed model building instructions, configurations, and troubleshooting, please see:** 
**[📖 Model Management Guide](model/README.md)**

## ⚙️ Platform Configuration

### PlatformIO Settings

#### ESP-SR Framework Support
```ini
# Use pioarduino platform with complete ESP-SR libraries
platform = https://github.com/pioarduino/platform-espressif32/archive/refs/tags/55.03.30-2.tar.gz
framework = arduino
```

#### Essential Build Flags
```ini
build_flags = 
    -DCORE_DEBUG_LEVEL=3                    # Debug logging
    -mfix-esp32-psram-cache-issue           # PSRAM stability
    -mfix-esp32-psram-cache-strategy=memw   # PSRAM cache strategy
    -DCONFIG_SR_VADN_VADNET1_MEDIUM=y       # Voice activity detection
    -DCONFIG_SR_WN_WN9S_HIESP=y             # "Hi ESP" wake word
    -DCONFIG_ESP32S3_INSTRUCTION_CACHE_32KB=y  # Cache optimization
    -DCONFIG_ESP32S3_DATA_CACHE_64KB=y      # Data cache size
    -DCONFIG_ESP32S3_DATA_CACHE_LINE_64B=y  # Cache line size
```

#### Custom Partition Table
```ini
board_build.partitions = hiesp.csv
```

## 🎤 Audio Configuration

### I2S Microphone Setup
- **Sample Rate**: 16kHz (required by ESP-SR)
- **Bit Depth**: 16-bit
- **Channels**: Mono
- **Frame Size**: 512 samples (32ms @ 16kHz)

### Pin Configuration
Microphone pin configurations are defined in [`include/config_mic.h`](include/config_mic.h):

**ESP32-S3-DevKitC-1-N16R8:**
```cpp
#define MIC_DIN 11    // I2S data pin (SD)
#define MIC_SCK 5     // I2S clock pin (SCK) 
#define MIC_WS  13    // I2S word select pin (WS/FS)
```

### Compatible I2S Microphones
- INMP441
- ICS-43434
- SPH0645

**Note**: ESP-SR requires I2S digital microphones. Analog microphones are not supported as ESP-SR expects direct I2S audio stream input.

## 🖥️ Display Integration

### Supported Displays
- **SSD1306** 128x64 OLED (I2C)
- **ST7789** 240x240 TFT (SPI)
- **Other U8g2 compatible displays**

### Configuration
```cpp
// U8g2 library integration
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
```

## 🔊 Audio Output (Optional)

### I2S Audio DAC
- **MAX98357A** I2S amplifier
- **UDA1334A** I2S DAC
- **Internal DAC** (ESP32-S3 built-in)

### TTS Voice Data
```bash
# Flash TTS voice data (if using text-to-speech)
esptool.py --baud 2000000 write_flash 0xCF0000 voice_data.bin
```

## 🐛 Troubleshooting

### Common Issues

#### "AFE_SR: ERROR: Please select wake words!"
**Cause**: Models not properly flashed or incorrect model partition address.

**Solution**:
1. Verify model binary is flashed to correct partition (0x710000)
2. Check partition table matches device configuration
3. Ensure models are built for correct wake word

#### I2S Initialization Failure
**Cause**: ESP-IDF driver compatibility issues.

**Solution**:
- Use the specified platform version with ESP-IDF v4.x compatibility
- Ensure PSRAM cache fix flags are enabled
- Check I2S pin configuration

#### Memory Issues / Boot Loops
**Cause**: PSRAM configuration problems.

**Solution**:
```ini
build_flags = 
    -mfix-esp32-psram-cache-issue
    -mfix-esp32-psram-cache-strategy=memw
```

#### Wake Word Not Detected
**Causes & Solutions**:
1. **Audio Input Issues**: Check microphone connections and I2S configuration
2. **Model Mismatch**: Ensure correct wake word model is flashed
3. **Noise/Environment**: Try different detection thresholds
4. **Sample Rate**: Verify 16kHz sample rate

### Debug Commands

#### Check Model Information
```bash
# List available models
find model/ -name "_MODEL_INFO_" -exec dirname {} \; | sort

# View model details
cat model/wakenet_model/wn9s_hiesp/_MODEL_INFO_
```

#### Monitor Serial Output
```bash
pio device monitor -e esp32-s3-devkitc1-n16r8 --baud 115200
```

#### Check Memory Usage
```bash
# Enable memory debugging in platformio.ini
build_flags = 
    ${env.build_flags}
    -DCORE_DEBUG_LEVEL=5
    -DDEBUG_ESP_SR_MEM=1
```

## 📚 Development References

### ESP-SR Documentation
- [ESP-SR GitHub Repository](https://github.com/espressif/esp-sr)
- [Wake Word Engine Guide](esp-sr/docs/en/wake_word_engine/README.rst)
- [Model Flashing Guide](esp-sr/docs/en/flash_model/README.rst)

### Arduino Framework Integration
- [Xu Jiwei's ESP32-S3 + ESP-SR Tutorial](https://xujiwei.com/blog/2025/04/esp32-arduino-esp-sr-tts/) - Comprehensive guide for Arduino framework integration

### Platform and Library Information
- [PioardUino Platform](https://github.com/pioarduino/platform-espressif32) - ESP32 platform with complete ESP-SR support
- [U8g2 Display Library](https://github.com/olikraus/u8g2) - Graphics library for monochrome displays

## 🔗 Related Documentation

For detailed model building instructions and advanced configurations, see:
- **[Model Management Guide](model/README.md)** - Comprehensive model building, flashing, and configuration guide

## 📄 License

This project incorporates the ESP-SR framework from Espressif Systems, which is licensed under the Espressif MIT License. See individual component licenses for details.

---