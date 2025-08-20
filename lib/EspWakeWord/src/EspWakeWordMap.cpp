#include "EspWakeWord.h"

const char* getWakeWordName(wakenet_state_t state) {
	switch (state) {
		case WAKENET_DETECTED:
			return "WAKENET_DETECTED";
		case WAKENET_NO_DETECT:
			return "WAKENET_NO_DETECT";
		case WAKENET_CHANNEL_VERIFIED:
			return "WAKENET_CHANNEL_VERIFIED";
		default:
			static char buf[32];
			sprintf(buf, "UNKNOWN, state = %3d", (int)state);
			return buf;
	}
}