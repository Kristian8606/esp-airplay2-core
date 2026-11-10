/* Calls what a firmware calls, so that every part of ap2_core and
 * apple_clock is compiled and linked (README.md, "What the firmware
 * calls"). */
#include <stdint.h>

#include "ap2_audio_crypto.h"
#include "ap2_hap.h"
#include "ap2_pairings.h"
#include "apple_clock.h"
#include "mdns_airplay.h"
#include "rtsp_remote.h"
#include "rtsp_server.h"

static void changed(bool has_admin) { (void)has_admin; }

void app_main(void) {
  apple_clock_start();
  ap2_hap_identity_init();
  mdns_airplay_init();
#ifdef CONFIG_AIRPLAY_HOMEKIT
  ap2_pairings_set_changed_cb(changed);
#else
  (void)changed;
#endif
  rtsp_server_start();

  uint32_t id = 0;
  rtsp_remote_status_t status;
  rtsp_remote_enqueue(0, &id);
  rtsp_remote_get_status(&status);
  (void)rtsp_remote_result_name(status.result);
  (void)ap2_pairings_has_admin();

  uint8_t packet[16] = {0}, out[16];
  ap2_audio_decrypt_rtp(NULL, packet + 12, 4, out, sizeof(out), packet, sizeof(packet));
  ap2_audio_decrypt_buffered(NULL, packet, sizeof(packet), out, sizeof(out));

  apple_clock_line_t line;
  uint64_t local = 0;
  apple_clock_get(&line);
  (void)apple_clock_at(&line, 0);
  (void)apple_clock_to_local(&line, 0, 0, &local);

  rtsp_server_begin_reset();
  rtsp_server_stop();
  (void)rtsp_server_is_idle();
}
