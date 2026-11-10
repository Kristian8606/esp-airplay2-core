/* Stubs of the firmware functions ap2_core calls. They only have to link. */
#include <stdio.h>
#include <string.h>

#include "airplay_identity.h"
#include "amp_control.h"
#include "ap2_port.h"
#include "sound_engine.h"

void sound_engine_set_format(const sound_format_t *format) { (void)format; }
void sound_engine_set_encryption(const sound_cipher_t *encrypt) { (void)encrypt; }
void sound_engine_set_stream_type(sound_stream_t type) { (void)type; }
esp_err_t sound_engine_start_stream(uint16_t data_port, uint16_t control_port,
                                    uint16_t tcp_port) {
  (void)data_port; (void)control_port; (void)tcp_port;
  return ESP_OK;
}
esp_err_t sound_engine_start_buffered(uint16_t tcp_port) { (void)tcp_port; return ESP_OK; }
void sound_engine_stop(void) {}
uint16_t sound_engine_get_buffered_port(void) { return 0; }
void sound_engine_set_volume_q15(int32_t volume_q15) { (void)volume_q15; }
void sound_engine_realtime_flush_to_rtp(uint32_t flush_rtp) { (void)flush_rtp; }
esp_err_t sound_engine_set_deferred_flush_range(uint32_t from_seq, uint32_t from_ts,
                                                uint32_t until_seq, uint32_t until_ts,
                                                bool timestamps_valid) {
  (void)from_seq; (void)from_ts; (void)until_seq; (void)until_ts;
  (void)timestamps_valid;
  return ESP_OK;
}
void sound_engine_set_immediate_flush(uint32_t until_seq, uint32_t until_ts) {
  (void)until_seq; (void)until_ts;
}
void sound_engine_pause(void) {}
void sound_engine_set_playing(bool playing) { (void)playing; }
void sound_engine_set_anchor_time(uint64_t clock_id, uint64_t network_time_ns,
                                  uint32_t rtp_time) {
  (void)clock_id; (void)network_time_ns; (void)rtp_time;
}
void sound_engine_set_client_control(uint32_t client_ip,
                                     uint16_t client_control_port) {
  (void)client_ip; (void)client_control_port;
}
void sound_engine_set_playout_latency_samples(uint32_t latency_samples) {
  (void)latency_samples;
}

void amp_control_session_connected(void) {}
void amp_control_session_disconnected(void) {}

uint32_t airplay_status_flags(void) { return 0x4; }
bool airplay_set_session_active(bool active) { (void)active; return false; }
void airplay_get_pairing_id(char *out, size_t len) {
  if (len) snprintf(out, len, "00000000-0000-4000-8000-000000000000");
}
void airplay_get_serial_number(char *out, size_t len) {
  if (len) snprintf(out, len, "000000000000");
}
void airplay_get_firmware_revision(char *out, size_t len) {
  if (len) snprintf(out, len, "0.0.0");
}

void ap2_port_homekit_reset_schedule(void) {}
esp_err_t ap2_port_device_mac(uint8_t mac[6]) {
  memset(mac, 0, 6);
  return ESP_OK;
}
