#include "app/callback_list.h"

// Event callback for SR system
void sr_event_callback(void *arg, sr_event_t event, int command_id, int phrase_id) {
    switch (event) {
        case SR_EVENT_WAKEWORD:
            Serial.println("🎙️ Wake word 'Hi ESP' detected!");
            if (notification) {
                notification->send(NOTIFICATION_DISPLAY, (void*)EVENT_DISPLAY_WAKEWORD);
            }
            // Switch to command listening mode
            sr_set_mode(SR_MODE_COMMAND);
            Serial.println("📞 Listening for commands...");
            break;
            
        case SR_EVENT_WAKEWORD_CHANNEL:
            Serial.printf("🎙️ Wake word detected on channel: %d\n", command_id);
            if (notification) {
                notification->send(NOTIFICATION_DISPLAY, (void*)EVENT_DISPLAY_WAKEWORD);
            }
            sr_set_mode(SR_MODE_COMMAND);
            break;
            
        case SR_EVENT_COMMAND:
            Serial.printf("✅ Command detected! ID=%d, Phrase=%d\n", command_id, phrase_id);
            
            // Map phrase_id to actual voice command (since phrase_id indexes the voice_commands array)
            if (phrase_id >= 0 && phrase_id < (sizeof(voice_commands) / sizeof(sr_cmd_t))) {
                const sr_cmd_t* cmd = &voice_commands[phrase_id];
                Serial.printf("   📝 You said: '%s'\n", cmd->str);
                Serial.printf("   � Phonetic: '%s'\n", cmd->phoneme);
                Serial.printf("   🆔 Command Group: %d, Phrase Index: %d\n", command_id, phrase_id);
            } else {
                Serial.println("   ❓ Unknown command mapping");
            }
            
            // Handle specific command groups based on command_id (from voice_commands array)
            switch (command_id) {
                case 0: 
                    Serial.println("💡 Action: Turning ON the light");
                    Serial.println("   🎯 Target: Light Control System (ON)");
                    // Add your light ON control logic here
                    if (notification) {
                        notification->send(NOTIFICATION_DISPLAY, (void*)"LIGHTS_ON");
                    }
                    break;
                case 1: 
                    Serial.println("💡 Action: Turning OFF the light");
                    Serial.println("   🎯 Target: Light Control System (OFF/DARK)");
                    // Add your light OFF control logic here
                    if (notification) {
                        notification->send(NOTIFICATION_DISPLAY, (void*)"LIGHTS_OFF");
                    }
                    break;
                case 2: 
                    Serial.println("🌀 Action: Starting fan");
                    Serial.println("   🎯 Target: Fan Control System (START)");
                    // Add your fan start control logic here
                    if (notification) {
                        notification->send(NOTIFICATION_DISPLAY, (void*)"FAN_START");
                    }
                    break;
                case 3: 
                    Serial.println("� Action: Stopping fan");
                    Serial.println("   🎯 Target: Fan Control System (STOP)");
                    // Add your fan stop control logic here
                    if (notification) {
                        notification->send(NOTIFICATION_DISPLAY, (void*)"FAN_STOP");
                    }
                    break;
                default: 
                    Serial.printf("❓ Unknown command ID: %d\n", command_id);
                    Serial.println("   📋 Available commands:");
                    for (int i = 0; i < (sizeof(voice_commands) / sizeof(sr_cmd_t)); i++) {
                        Serial.printf("      [%d] Group %d: '%s' (%s)\n", 
                                    i,
                                    voice_commands[i].command_id, 
                                    voice_commands[i].str, 
                                    voice_commands[i].phoneme);
                    }
                    break;
            }
            
            // Return to wake word mode after command
            sr_set_mode(SR_MODE_WAKEWORD);
            Serial.println("🔄 Returning to wake word detection mode");
            break;
            
        case SR_EVENT_TIMEOUT:
            Serial.println("⏰ Command timeout - returning to wake word mode");
            Serial.println("   💭 No command detected within timeout period");
            Serial.println("   🔄 Say 'Hi ESP' to activate again");
            sr_set_mode(SR_MODE_WAKEWORD);
            break;
            
        default:
            Serial.printf("❓ Unknown SR event: %d\n", event);
            Serial.println("   📚 Known events:");
            Serial.println("      SR_EVENT_WAKEWORD: Wake word detected");
            Serial.println("      SR_EVENT_WAKEWORD_CHANNEL: Multi-channel wake word");
            Serial.println("      SR_EVENT_COMMAND: Voice command detected");
            Serial.println("      SR_EVENT_TIMEOUT: Command timeout occurred");
            break;
    }
}