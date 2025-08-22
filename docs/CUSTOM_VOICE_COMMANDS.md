# Custom Voice Commands Guide

This guide explains how to add and customize voice commands in the ESP32-S3 Wake Word Detection system using ESP-SR framework.

## Overview

The ESP-SR system uses **MultiNet** for command recognition, which requires:
1. **Text Command**: The actual words you speak
2. **Phonetic Representation**: How the words sound phonetically
3. **Command ID**: Numeric identifier for grouping related commands

## Voice Command Structure

```cpp
static const sr_cmd_t voice_commands[] = {
    {command_id, "spoken text", "phonetic representation"},
    // Example:
    {0, "Turn on the light", "TkN nN jc LiT"},
};
```

### Command Components:
- **command_id**: Integer (0-255) that groups similar commands
- **spoken text**: Exact words the user should speak
- **phonetic**: ESP-SR phonetic representation of the spoken text

## Phonetic Conversion Rules

### Basic Phonetic Mapping

| Letter | Phonetic | Example Word | Phonetic |
|--------|----------|--------------|----------|
| A | c | Apple | cPcL |
| B | B | Book | BkK |
| C | K | Cat | KcT |
| D | D | Dog | DnG |
| E | c / e | End | eND |
| F | F | Fish | FgS |
| G | G | Go | Gb |
| H | H | House | HdS |
| I | g / i | In | gN |
| J | j | Jump | jcMP |
| K | K | Key | Ki |
| L | L | Light | LiT |
| M | M | Make | MeK |
| N | N | No | Nb |
| O | b / o | On | nN |
| P | P | Put | PkT |
| Q | K | Quick | KWgK |
| R | R | Red | ReD |
| S | S | Stop | STnP |
| T | T | Turn | TkN |
| U | k / u | Up | kP |
| V | V | Very | VeRi |
| W | W | With | WgT |
| X | KS | Box | BnKS |
| Y | Y / i | Yes | YeS |
| Z | S | Zero | SiRb |

### Special Combinations

| Combination | Phonetic | Example |
|-------------|----------|---------|
| TH | j | The → jc |
| SH | S | Show → Sb |
| CH | p | Chair → peR |
| CK | K | Back → BcK |
| NG | N | Ring → RgN |
| AR | nR | Start → STnRT |
| ER | cR | Water → WnTcR |
| OR | bR | For → FbR |
| OO | k | Book → BkK |
| EE | i | See → Si |

## Step-by-Step Guide to Add Custom Commands

### Step 1: Choose Your Command Text

Decide what phrases users will speak:
```
"Open garage door"
"Close garage door" 
"Turn on music"
"Volume up"
"Volume down"
```

### Step 2: Convert to Phonetics

Use the phonetic mapping rules:

| Command | Phonetic Breakdown | Final Phonetic |
|---------|-------------------|----------------|
| "Open garage door" | O-pen ga-rage do-or | bPcN GcRnj DbR |
| "Close garage door" | Clo-se ga-rage do-or | KLbS GcRnj DbR |
| "Turn on music" | Turn on mu-sic | TkN nN MYkSgK |
| "Volume up" | Vo-lu-me up | VnLYkM kP |
| "Volume down" | Vo-lu-me down | VnLYkM DdN |

### Step 3: Assign Command IDs

Group related commands with the same ID:
```cpp
// Garage control = ID 0
// Music control = ID 1  
// Volume control = ID 2
```

### Step 4: Add to constants.h

```cpp
static const sr_cmd_t voice_commands[] = {
    // Garage Control (ID: 0)
    {0, "Open garage door", "bPcN GcRnj DbR"},
    {0, "Open garage", "bPcN GcRnj"},
    
    // Garage Control (ID: 1) 
    {1, "Close garage door", "KLbS GcRnj DbR"},
    {1, "Close garage", "KLbS GcRnj"},
    
    // Music Control (ID: 2)
    {2, "Turn on music", "TkN nN MYkSgK"},
    {2, "Play music", "PLe MYkSgK"},
    
    // Volume Control (ID: 3)
    {3, "Volume up", "VnLYkM kP"},
    {3, "Louder", "LdDcR"},
    
    // Volume Control (ID: 4)
    {4, "Volume down", "VnLYkM DdN"},
    {4, "Quieter", "KWiTcR"},
};
```

### Step 5: Update Command Handler

Edit `src/app/callback/sr_event.cpp`:

```cpp
switch (command_id) {
    case 0: 
        Serial.println("🚪 Action: Opening garage door");
        // Add your garage open logic here
        break;
        
    case 1: 
        Serial.println("🚪 Action: Closing garage door");
        // Add your garage close logic here
        break;
        
    case 2: 
        Serial.println("🎵 Action: Starting music");
        // Add your music start logic here
        break;
        
    case 3: 
        Serial.println("🔊 Action: Volume up");
        // Add your volume up logic here
        break;
        
    case 4: 
        Serial.println("🔉 Action: Volume down");
        // Add your volume down logic here
        break;
}
```

## Advanced Phonetic Conversion

### Tools for Phonetic Conversion

1. **Manual Method**: Use the mapping table above
2. **ESP-SR Examples**: Study existing models in `model/` directory
3. **Pronunciation Guides**: Use online pronunciation dictionaries

### Testing Your Phonetics

1. **Build and Upload**: Flash your ESP32 with new commands
2. **Monitor Serial**: Watch for command detection logs
3. **Adjust Phonetics**: If detection fails, modify phonetic representation
4. **Iterate**: Test different phonetic combinations

### Common Phonetic Patterns

```cpp
// Common word patterns
"Turn" → "TkN"
"Start" → "STnRT" 
"Stop" → "STnP"
"On" → "nN"
"Off" → "eF"
"Up" → "kP"
"Down" → "DdN"
"Light" → "LiT"
"Fan" → "FaN"
"Music" → "MYkSgK"
"Volume" → "VnLYkM"
"Open" → "bPcN"
"Close" → "KLbS"
```

## Best Practices

### 1. Command Design
- **Keep commands 2-4 words**: Easier to recognize
- **Use common words**: Avoid technical jargon
- **Make commands distinct**: Avoid similar-sounding commands
- **Test pronunciation**: Ensure natural speech patterns

### 2. Phonetic Accuracy
- **Start simple**: Test basic words first
- **Use existing examples**: Copy patterns from working commands
- **Account for accents**: Test with different speakers
- **Iterate quickly**: Adjust based on testing results

### 3. Command Organization
- **Group by function**: Use same command_id for related actions
- **Logical numbering**: Keep command IDs sequential
- **Document mapping**: Comment your command groups

### 4. Testing Strategy
- **Test in quiet environment**: Minimize background noise
- **Multiple speakers**: Test with different voices
- **Various distances**: Test microphone positioning
- **Real conditions**: Test in actual usage environment

## Example: Smart Home Commands

Here's a complete example for smart home control:

```cpp
static const sr_cmd_t voice_commands[] = {
    // Lighting Control (ID: 0-1)
    {0, "Turn on lights", "TkN nN LiTS"},
    {0, "Lights on", "LiTS nN"},
    {0, "Illuminate", "gLYkMgNeT"},
    
    {1, "Turn off lights", "TkN eF LiTS"},
    {1, "Lights off", "LiTS eF"},
    {1, "Go dark", "Gb DnRK"},
    
    // Climate Control (ID: 2-3)
    {2, "Start air conditioning", "STnRT eR KcNDgSYNgN"},
    {2, "Turn on AC", "TkN nN eSi"},
    
    {3, "Stop air conditioning", "STnP eR KcNDgSYNgN"},
    {3, "Turn off AC", "TkN eF eSi"},
    
    // Security (ID: 4-5)
    {4, "Arm security", "nRM SgKYkRgTi"},
    {4, "Lock house", "LnK HdS"},
    
    {5, "Disarm security", "DgSnRM SgKYkRgTi"},
    {5, "Unlock house", "kNLnK HdS"},
    
    // Entertainment (ID: 6-9)
    {6, "Play music", "PLe MYkSgK"},
    {6, "Start music", "STnRT MYkSgK"},
    
    {7, "Stop music", "STnP MYkSgK"},
    {7, "Pause music", "PbS MYkSgK"},
    
    {8, "Volume up", "VnLYkM kP"},
    {8, "Louder", "LdDcR"},
    
    {9, "Volume down", "VnLYkM DdN"},
    {9, "Quieter", "KWiTcR"},
};
```

## Troubleshooting

### Common Issues

1. **Command Not Recognized**
   - Check phonetic accuracy
   - Verify microphone functionality
   - Test in quieter environment
   - Speak clearly and at normal pace

2. **Wrong Command Triggered**
   - Commands too similar phonetically
   - Adjust phonetic representation
   - Make commands more distinct

3. **Inconsistent Recognition**
   - Background noise interference
   - Microphone positioning
   - Speaking distance variations

### Debug Steps

1. **Enable Verbose Logging**: Check serial output for detection attempts
2. **Test Individual Words**: Break complex commands into parts
3. **Compare with Working Examples**: Use existing phonetics as reference
4. **Microphone Testing**: Verify audio input quality

## Performance Considerations

- **Maximum Commands**: ESP-SR supports up to ~200 commands
- **Memory Usage**: More commands = more RAM usage
- **Recognition Speed**: Fewer commands = faster recognition
- **Model Size**: Affects SPIFFS storage requirements

## References

- [ESP-SR Official Documentation](https://docs.espressif.com/projects/esp-sr/)
- [MultiNet Command Recognition](https://github.com/espressif/esp-sr)
- [Phonetic Examples in ESP-SR Models](../model/)
- [Project Callback Implementation](../src/app/callback/sr_event.cpp)

---

*This documentation is part of the ESP32-S3 Wake Word Detection project. For issues or improvements, please check the project repository.*
