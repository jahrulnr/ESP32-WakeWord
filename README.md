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
│   │   ├── callback/          # ESP-SR event callbacks
│   │   ├── display/           # Display functions
│   │   ├── tasks/             # FreeRTOS task implementations
│   │   ├── callback_list.h    # Callback function declarations
│   │   ├── display_list.h     # Display function declarations
│   │   ├── runTasks.cpp       # Task creation and management
│   │   └── tasks.h            # Task handle declarations
│   └── boot/                  # Boot sequence and initialization
│       ├── init.h             # Global variable declarations
│       ├── setup.cpp          # System setup functions
│       └── constants.h        # Application constants
├── include/                   # Project header files
│   └── app_config.h           # Hardware pin and config definitions
├── lib/                       # Custom libraries
│   ├── Microphone/            # I2S microphone interface (ESP-IDF v5+ STD API)
│   ├── Display/               # U8g2 display wrapper
│   ├── FaceDisplay/           # Animated face graphics for OLED
│   ├── Notification/          # FreeRTOS inter-task messaging system
│   └── MochiDisplay/          # Display character animations
├── model/                     # ESP-SR model management
│   ├── pack_model.py          # Model packing script
│   ├── movemodel.py           # ESP-IDF model mover
│   ├── target/                # Built model output
│   ├── wakenet_model/         # Wake word detection models
│   ├── multinet_model/        # Speech command recognition
│   ├── vadnet_model/          # Voice activity detection
│   └── nsnet_model/           # Noise suppression
├── platformio.ini             # PlatformIO configuration
├── hiesp.csv                  # Custom partition table
└── boards/                    # Custom board definitions
```

## 🔧 Hardware Configuration

### Supported Boards

#### ESP32-S3-DevKitC-1-N16R8
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
Microphone pin configurations are defined in [`include/app_config.h`](include/app_config.h):

**ESP32-S3-DevKitC-1-N16R8:**
```cpp
#define MIC_DIN 11    // I2S data pin (SD)
#define MIC_SCK 5     // I2S clock pin (SCK) 
#define MIC_WS 13     // I2S word select pin (WS/FS)
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

### Display System Architecture
The project includes multiple display modes for different types of feedback:

#### 1. FaceDisplay System (Animated Faces)
```cpp
// Animated face with expressions and behaviors
Face* faceDisplay = new Face(display, 128, 64, 40);
faceDisplay->Expression.GoTo_Awe();       // Set expression
faceDisplay->LookFront();                 // Set gaze direction
faceDisplay->Update();                    // Animate and render
```

**Available Expressions:**
- Normal, Happy, Sad, Angry, Surprised, Focused
- Awe, Glee, Worried, Annoyed, Skeptic, Sleepy
- Frustrated, Unimpressed, Suspicious, Squint, Furious, Scared

#### 2. MochiDisplay System (Character Animation)
```cpp
// Pre-rendered frame-based animation (90 frames)
#include "Mochi.h"
Mochi::drawFrame(display);  // Plays animated character sequence
```

#### 3. Custom Display Functions
- `displaySoundDetector()` - Real-time microphone level visualization
- `displayListening()` - Animated microphone icon during command listening
- `displayCommand(command)` - Command confirmation feedback

### Configuration
```cpp
// U8g2 library integration in lib/Display/
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

## ⚡ FreeRTOS Task Architecture

### Core Tasks
The application runs two main FreeRTOS tasks with specific priorities and core assignments:

#### 1. Speech Recognition Task (Core 0, Priority 8)
```cpp
// Monitors ESP-SR system health and handles SR commands
speechRecognitionTask()
```
- **Function**: System health monitoring, memory checks, SR system status
- **Stack**: 4KB (1024 * 4)
- **Features**: Periodic heap monitoring, SR pause/resume commands
- **Monitoring**: 30-second health reports with memory statistics

#### 2. Display Task (Core 1, Priority 19)  
```cpp
// Handles all display updates and animations
displayTask()
```
- **Function**: UI rendering, animation updates, notification handling
- **Stack**: 4KB (1024 * 4) 
- **Update Rate**: 33ms (30 FPS)
- **Modes**: Sound detector, Face animations, Mochi character animations

### Inter-Task Communication
```cpp
// FreeRTOS-native notification system
notification->send(NOTIFICATION_DISPLAY, (void*)EVENT_DISPLAY_WAKEWORD);
void* event = notification->consume(NOTIFICATION_DISPLAY, timeout_ticks);
```

### Task Priorities and Core Assignment
- **Core 0**: Speech recognition processing (lower priority, CPU-intensive)
- **Core 1**: Display updates (higher priority, real-time UI responsiveness)
- **Priority 19 > 8**: Ensures smooth UI even during heavy SR processing

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
- Use the specified platform version with ESP-IDF v5.x compatibility
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