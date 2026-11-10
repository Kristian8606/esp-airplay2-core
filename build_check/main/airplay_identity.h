#pragma once
/* Stub of the firmware's identity: the part ap2_core uses. The values are
 * those of the firmwares (stereo, HomeKit as configured). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sdkconfig.h"

#define AIRPLAY_MODEL "AudioAccessory6,1"
#ifdef CONFIG_AIRPLAY_HOMEKIT
#define AIRPLAY_SOURCE_VERSION "366.0"
#else
#define AIRPLAY_SOURCE_VERSION "377.40.00"
#endif
#define AIRPLAY_SERVER_HEADER "AirTunes/" AIRPLAY_SOURCE_VERSION
#define AIRPLAY_PROTOVERS "1.1"
#define AIRPLAY_PROTOCOL_VERSION 2

uint32_t airplay_status_flags(void);
bool airplay_set_session_active(bool active);

#define AIRPLAY_FEATURES_HI 0x1C340
#ifdef CONFIG_AIRPLAY_HOMEKIT
#define AIRPLAY_FEATURES_LO 0x401D4A00
#else
#define AIRPLAY_FEATURES_LO 0x401C4A00
#endif

#define AIRPLAY_FMT_ALAC_44100_S16_2 (1ULL << 18)
#define AIRPLAY_FMT_ALAC_48000_S24_2 (1ULL << 21)
#define AIRPLAY_FMT_AAC_LC_44100_2   (1ULL << 22)
#define AIRPLAY_FMT_AAC_LC_48000_2   (1ULL << 23)
#define AIRPLAY_FMT_AAC_LC_48000_5_1 (1ULL << 39)
#define AIRPLAY_FMT_AAC_LC_48000_7_1 (1ULL << 40)
#define AIRPLAY_SUPPORTED_AUDIO_STREAM AIRPLAY_FMT_ALAC_44100_S16_2
#define AIRPLAY_SUPPORTED_BUFFER_STREAM                          \
  (AIRPLAY_FMT_ALAC_44100_S16_2 | AIRPLAY_FMT_ALAC_48000_S24_2 | \
   AIRPLAY_FMT_AAC_LC_44100_2 | AIRPLAY_FMT_AAC_LC_48000_2)

#define AIRPLAY_PAIRING_ID_LEN 37
void airplay_get_pairing_id(char *out, size_t len);

#define AIRPLAY_SERIAL_LEN 13
void airplay_get_serial_number(char *out, size_t len);
void airplay_get_firmware_revision(char *out, size_t len);
