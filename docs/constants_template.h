#ifndef BOOT_CONSTANTS_H
#define BOOT_CONSTANTS_H

#include "esp32-hal-sr.h"

// Define voice commands (phonetic representations)
// Template for adding custom voice commands
// 
// Structure: {command_id, "spoken text", "phonetic representation"}
// 
// Command ID Groups:
// 0-1:   Lighting control
// 2-3:   Fan control  
// 4-5:   Music control
// 6-7:   Volume control
// 8-9:   Smart home devices
// 10+:   Your custom commands
//
// Phonetic Conversion Tips:
// - Use PHONETIC_REFERENCE.md for common word mappings
// - Test with simple words first
// - Add multiple variations for better recognition
// - Keep commands 2-4 words for best results

static const sr_cmd_t voice_commands[] = {
    // === LIGHTING CONTROL (ID: 0-1) ===
    {0, "Turn on the light", "TkN nN jc LiT"},
    {0, "Switch on the light", "SWgp nN jc LiT"},
    {0, "Lights on", "LiTS nN"},
    {0, "Illuminate", "gLYkMgNeT"},
    
    {1, "Turn off the light", "TkN eF jc LiT"},
    {1, "Switch off the light", "SWgp eF jc LiT"},
    {1, "Lights off", "LiTS eF"},
    {1, "Go dark", "Gb DnRK"},

    // === FAN CONTROL (ID: 2-3) ===
    {2, "Start fan", "STnRT FaN"},
    {2, "Turn on fan", "TkN nN FaN"},
    {2, "Fan on", "FaN nN"},
    
    {3, "Stop fan", "STnP FaN"},
    {3, "Turn off fan", "TkN eF FaN"},
    {3, "Fan off", "FaN eF"},

    // === MUSIC CONTROL (ID: 4-5) ===
    {4, "Play music", "PLe MYkSgK"},
    {4, "Start music", "STnRT MYkSgK"},
    {4, "Music on", "MYkSgK nN"},
    
    {5, "Stop music", "STnP MYkSgK"},
    {5, "Pause music", "PbS MYkSgK"},
    {5, "Music off", "MYkSgK eF"},

    // === VOLUME CONTROL (ID: 6-7) ===
    {6, "Volume up", "VnLYkM kP"},
    {6, "Louder", "LdDcR"},
    {6, "Increase volume", "gNKRiS VnLYkM"},
    
    {7, "Volume down", "VnLYkM DdN"},
    {7, "Quieter", "KWiTcR"},
    {7, "Decrease volume", "DiKRiS VnLYkM"},

    // === SMART HOME DEVICES (ID: 8-9) ===
    {8, "Open garage", "bPcN GcRnj"},
    {8, "Garage open", "GcRnj bPcN"},
    
    {9, "Close garage", "KLbS GcRnj"},
    {9, "Garage close", "GcRnj KLbS"},

    // === CUSTOM COMMANDS (ID: 10+) ===
    // Add your custom commands here following the pattern above
    // 
    // Example templates:
    // {10, "Your command text", "YkR KcMnND TeKST"},
    // {11, "Another command", "cNjcR KcMnND"},
    
    // Uncomment and modify the examples below:
    
    // {10, "Turn on TV", "TkN nN TiVi"},
    // {10, "TV on", "TiVi nN"},
    // 
    // {11, "Turn off TV", "TkN eF TiVi"},
    // {11, "TV off", "TiVi eF"},
    // 
    // {12, "Lock door", "LnK DbR"},
    // {12, "Secure house", "SgKYkR HdS"},
    // 
    // {13, "Unlock door", "kNLnK DbR"},
    // {13, "Open door", "bPcN DbR"},
    // 
    // {14, "Start coffee", "STnRT KnFi"},
    // {14, "Make coffee", "MeK KnFi"},
    // 
    // {15, "Check weather", "peK WejcR"},
    // {15, "Weather report", "WejcR RgPbRT"},
};

// Notification channels for inter-task communication
static const char* NOTIFICATION_WAKEWORD = "wakeword";
static const char* NOTIFICATION_DISPLAY = "display";
static const char* NOTIFICATION_SPEAKER = "speaker";
static const char* NOTIFICATION_COMMAND = "command";

// Display Events - sent to display task
static const char* EVENT_DISPLAY_WAKEWORD = "display_wakeword";
static const char* EVENT_DISPLAY_COMMAND = "display_command";
static const char* EVENT_DISPLAY_LISTENING = "display_listening";

// Speech Recognition Events
static const char* EVENT_SR_WAKEWORD = "sr_wakeword";
static const char* EVENT_SR_COMMAND = "sr_command";
static const char* EVENT_SR_TIMEOUT = "sr_timeout";

#endif

/*
 * USAGE INSTRUCTIONS:
 * 
 * 1. Choose your command text (what users will say)
 * 2. Convert to phonetics using PHONETIC_REFERENCE.md
 * 3. Assign appropriate command_id (group related commands)
 * 4. Add command handling in src/app/callback/sr_event.cpp
 * 5. Build, upload, and test
 * 
 * TESTING TIPS:
 * 
 * - Start with simple, common words
 * - Test in a quiet environment first
 * - Speak clearly at normal speed
 * - Add multiple variations for the same action
 * - Monitor serial output for debugging
 * 
 * COMMON ISSUES:
 * 
 * - Command not recognized: Check phonetic accuracy
 * - Wrong command triggered: Make phonetics more distinct
 * - Inconsistent recognition: Test microphone positioning
 * 
 * For detailed help, see docs/CUSTOM_VOICE_COMMANDS.md
 */
