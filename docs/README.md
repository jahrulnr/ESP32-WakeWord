# ESP32-S3 Wake Word Detection - Documentation

This directory contains documentation for customizing and extending the ESP32-S3 Wake Word Detection system.

## Files Overview

### 📖 [CUSTOM_VOICE_COMMANDS.md](CUSTOM_VOICE_COMMANDS.md)
**Complete guide for adding custom voice commands**
- Step-by-step instructions
- Phonetic conversion rules  
- Command structure explanation
- Best practices and troubleshooting
- Real-world examples

### 📝 [PHONETIC_REFERENCE.md](PHONETIC_REFERENCE.md)
**Quick reference for phonetic conversions**
- Vowel and consonant mapping tables
- Common word patterns
- Frequently used command examples
- Testing tips

### 🛠️ [constants_template.h](constants_template.h)
**Template file for adding custom commands**
- Pre-organized command categories
- Inline documentation
- Example commands (commented out)
- Copy this to replace `src/boot/constants.h`

## Quick Start Guide

### 1. **Read the Documentation**
Start with `CUSTOM_VOICE_COMMANDS.md` for a complete understanding.

### 2. **Plan Your Commands**
- Decide what voice commands you want
- Keep commands 2-4 words long
- Group related commands with same ID

### 3. **Convert to Phonetics**
Use `PHONETIC_REFERENCE.md` to convert your words:
```
"Turn on light" → "TkN nN LiT"
"Play music" → "PLe MYkSgK"
```

### 4. **Use the Template**
Copy `constants_template.h` to `src/boot/constants.h` and customize.

### 5. **Add Command Handling**
Edit `src/app/callback/sr_event.cpp` to handle your command IDs.

### 6. **Test and Iterate**
- Build and upload firmware
- Test voice commands
- Adjust phonetics if needed
- Repeat until satisfied

## Common Examples

### Smart Home Commands
```cpp
{10, "Turn on TV", "TkN nN TiVi"},
{11, "Lock door", "LnK DbR"},
{12, "Start coffee", "STnRT KnFi"},
```

### Entertainment Commands  
```cpp
{13, "Play playlist", "PLe PLeST"},
{14, "Next song", "NeKST SnN"},
{15, "Previous song", "PRiVYkS SnN"},
```

### Climate Control
```cpp
{16, "Heat up", "HiT kP"},
{17, "Cool down", "KkL DdN"},
{18, "Set temperature", "SeT TeMPRcpR"},
```

## Troubleshooting

### Command Not Recognized
1. Check phonetic accuracy against reference tables
2. Test microphone in quiet environment
3. Speak clearly at normal pace
4. Try simpler alternative words

### Wrong Command Triggered
1. Make commands more distinct phonetically
2. Avoid similar-sounding commands
3. Add distinguishing words

### Inconsistent Recognition
1. Check microphone positioning
2. Test speaking distance
3. Reduce background noise
4. Consider microphone quality

## Support

- **Main Project**: See project README.md
- **ESP-SR Documentation**: [Official ESP-SR Docs](https://docs.espressif.com/projects/esp-sr/)
- **Issues**: Check project repository for known issues

## Contributing

Found an error or want to improve the documentation?
1. Test your changes thoroughly
2. Follow the existing documentation style
3. Add examples when helpful
4. Submit changes via the project repository

---

**Happy voice commanding! 🎤**
