# ESP32 Wake Word Detection

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-green.svg)
![Framework](https://img.shields.io/badge/framework-Arduino-blue.svg)

An ESP32-S3 based wake word detection system using Espressif's ESP-SR framework. Features voice control with animated face display feedback and supports both I2S digital and analog microphones.

## Features

- **Wake Word Detection**: Activate with "Hi ESP" command
- **Voice Commands**: Control lights and fans with natural language
- **Animated Display**: OLED display with expressive face animations
- **Dual Microphone Support**: I2S digital or analog microphone options
- **FreeRTOS Multi-Core**: Optimized task distribution across both cores

## Hardware Requirements

- ESP32-S3-DevKitC-1-N16R8 (16MB Flash, 8MB PSRAM)
- OLED Display (I2C, 128x64)
- Microphone Options:
  - I2S Digital: INMP441, ICS-43434, or SPH0645
  - Analog: MAX9814 or similar with gain control

## Dependencies

- Platform: ESP32 Arduino (Custom platform with ESP-SR support)
- Libraries:
  - U8g2 (OLED Graphics)
  - ESP-SR (Speech Recognition)
  - ESP-Skainet (Wake Word Models)

## Installation

1. Install PlatformIO (VSCode Extension or CLI)

2. Clone the repository:
   ```bash
   git clone https://github.com/jahrulnr/ESP32-WakeWord.git
   cd ESP32-WakeWord
   ```

3. Configure hardware settings in `include/app_config.h`:
   ```cpp
   // Select microphone type
   #define MIC_TYPE MIC_TYPE_I2S  // or MIC_TYPE_ANALOG
   
   // Configure pins according to your wiring
   #define MIC_SCK GPIO_NUM_41    // I2S: SCK
   #define MIC_WS  GPIO_NUM_42    // I2S: WS
   #define MIC_DIN GPIO_NUM_2     // I2S: SD
   // Or for analog microphone
   #define MIC_OUT GPIO_NUM_4     // Analog: Output
   #define MIC_GAIN GPIO_NUM_38   // Analog: Gain
   ```

4. Build and upload:
   ```bash
   pio run -t upload
   ```

## Voice Commands

1. Wake Word:
   - Say "Hi ESP" to activate command mode

2. Light Control:
   ```
   "Turn on the light" / "Switch on the light"
   "Turn off the light" / "Switch off the light" / "Go dark"
   ```

3. Fan Control:
   ```
   "Start fan"
   "Stop fan"
   ```

## Project Structure

```
src/
├── main.cpp              # Entry point
├── boot/                 # System initialization
│   ├── setup.cpp        # Hardware setup sequence
│   └── constants.h      # Voice commands & constants
├── app/                 # Application logic
│   ├── tasks/          # FreeRTOS tasks
│   ├── callback/       # ESP-SR callbacks
│   └── display/        # Display functions
lib/                    # Custom libraries
├── Display/            # Display abstraction
├── FaceDisplay/        # Animated face system
├── Microphone/        # Microphone interfaces
└── Notification/      # Inter-task communication
```

## ⚡ Architecture

### Task Distribution
- **Core 0**: Speech recognition processing
  - Priority 8
  - 4KB stack
  - Handles ESP-SR system and audio processing

- **Core 1**: Display and animations
  - Priority 19
  - 4KB stack
  - Manages UI updates and face animations

### Memory Configuration
- Custom partition table (`hiesp.csv`)
- 8.9MB dedicated to model storage
- PSRAM optimization for ESP32-S3

## Model Management

For detailed instructions on building, packaging, and flashing ESP-SR models, see the [ESP-SR Model Management Guide](model/README.md). The guide includes:

- Model structure and organization
- Build and flash instructions
- Pre-built configurations
- Troubleshooting common model issues
- ESP-SR and I2S integration tips

## Troubleshooting

1. **Speech Recognition Issues**
   - Verify 16kHz sample rate configuration
   - Check microphone signal levels
   - Monitor heap usage with serial output

2. **Build Errors**
   - Clean build: `pio run -t clean`
   - Verify ESP-SR platform compatibility
   - Check PSRAM configuration

3. **Display Problems**
   - Verify I2C pins in `app_config.h`
   - Check OLED address/configuration
   - Monitor display task stack usage

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [ESP-SR Framework](https://github.com/espressif/esp-sr) by Espressif
- [U8g2 Library](https://github.com/olikraus/u8g2)
- Original face animations based on [ESP32-Eyes](https://github.com/playfultechnology/esp32-eyes)
