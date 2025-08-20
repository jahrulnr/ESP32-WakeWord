#ifndef BOOT_CONSTANTS_H
#define BOOT_CONSTANTS_H

#include "esp32-hal-sr.h"

// Define voice commands (phonetic representations)
static const sr_cmd_t voice_commands[] = {
	{0, "Turn on the light", "TkN nN jc LiT"},
	{0, "Switch on the light", "SWgp nN jc LiT"},
	{1, "Turn off the light", "TkN eF jc LiT"},
	{1, "Switch off the light", "SWgp eF jc LiT"},
	{1, "Go dark", "Gb DnRK"},
	{2, "Start fan", "STnRT FaN"},
	{3, "Stop fan", "STnP FaN"},
};

static const char* NOTIFICATION_WAKEWORD = "wakeword";
static const char* NOTIFICATION_DISPLAY = "display";
static const char* NOTIFICATION_SPEAKER = "speaker";
static const char* NOTIFICATION_COMMAND = "command";

// Display Events
static const char* EVENT_DISPLAY_WAKEWORD = "display_wakeword";
static const char* EVENT_DISPLAY_COMMAND = "display_command";
static const char* EVENT_DISPLAY_LISTENING = "display_listening";

// SR Events
static const char* EVENT_SR_WAKEWORD = "sr_wakeword";
static const char* EVENT_SR_COMMAND = "sr_command";
static const char* EVENT_SR_TIMEOUT = "sr_timeout";

#endif