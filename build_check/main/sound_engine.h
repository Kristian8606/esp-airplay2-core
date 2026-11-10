#pragma once
/* Stub of the firmware's audio engine: the part ap2_core uses. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

/* Buffered audio capacity told to the sender in SETUP (type 103). */
#define AP2_BUFFERED_AUDIO_ADVERTISED_BYTES (6U * 1024U * 1024U)

typedef struct {
  char codec[32];
  int sample_rate;
  int channels;
  int bits_per_sample;
  int frame_size;
} sound_format_t;

typedef enum {
  SOUND_CIPHER_NONE = 0,
  SOUND_CIPHER_CHACHA20_POLY1305
} sound_cipher_kind_t;

typedef struct {
  sound_cipher_kind_t type;
  uint8_t key[32];
  uint8_t iv[16];
  size_t key_len;
} sound_cipher_t;

typedef enum {
  SOUND_STREAM_NONE = 0,
  SOUND_STREAM_REALTIME = 96,
  SOUND_STREAM_BUFFERED = 103
} sound_stream_t;

void sound_engine_set_format(const sound_format_t *format);
void sound_engine_set_encryption(const sound_cipher_t *encrypt);
void sound_engine_set_stream_type(sound_stream_t type);
esp_err_t sound_engine_start_stream(uint16_t data_port, uint16_t control_port,
                                    uint16_t tcp_port);
esp_err_t sound_engine_start_buffered(uint16_t tcp_port);
void sound_engine_stop(void);
uint16_t sound_engine_get_buffered_port(void);
void sound_engine_set_volume_q15(int32_t volume_q15);
void sound_engine_realtime_flush_to_rtp(uint32_t flush_rtp);
esp_err_t sound_engine_set_deferred_flush_range(uint32_t from_seq, uint32_t from_ts,
                                                uint32_t until_seq, uint32_t until_ts,
                                                bool timestamps_valid);
void sound_engine_set_immediate_flush(uint32_t until_seq, uint32_t until_ts);
void sound_engine_pause(void);
void sound_engine_set_playing(bool playing);
void sound_engine_set_anchor_time(uint64_t clock_id, uint64_t network_time_ns,
                                  uint32_t rtp_time);
void sound_engine_set_client_control(uint32_t client_ip,
                                     uint16_t client_control_port);
void sound_engine_set_playout_latency_samples(uint32_t latency_samples);
