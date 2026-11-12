#pragma once

/* Apple clock: owns UDP 319/320 and estimates Apple's time from two-way
 * Sync/Delay_Req exchanges. No passive estimator or fallback clock. */
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

typedef struct {
  bool ready;
  bool realtime;
  uint64_t source_clock_id, grandmaster_clock_id;
  uint32_t epoch;
  /* Apple = base_apple + (local - base_local) * (1 + rate_ppb / 1e9). */
  int64_t base_local_ns, base_apple_ns;
  int32_t rate_ppb;
  int64_t ready_since_ns;
  uint32_t samples; /* quality-checked two-way observations, not seconds */
  uint32_t points;
} apple_clock_line_t;

esp_err_t apple_clock_start(void);
/* SETUP selects the sender (IP in network byte order) and media mode.
 * Releasing a session leaves its clock running for the next track. */
void apple_clock_select(uint32_t master_ip, bool realtime);
void apple_clock_release(void);
void apple_clock_get(apple_clock_line_t *out);
int64_t apple_clock_at(const apple_clock_line_t *line, int64_t local_ns);
bool apple_clock_to_local(const apple_clock_line_t *line, uint64_t clock_id,
                          uint64_t apple_ns, uint64_t *local_ns);

/* Optional receive-time hook. A firmware whose packets wait on the way to
 * its sockets (the P4 Gateway, whose Wi-Fi is a co-processor that time-stamps
 * PTP packets as they come from the air) can give a better local time
 * (esp_timer, ns) for a received Sync. It is asked when that Sync's exchange
 * completes (its Delay_Resp arrived), so it need not wait for anything:
 * type is the PTP messageType (0), seq the sequenceId, port_id the 10-byte
 * sourcePortIdentity, socket_ns the time taken here at recvfrom(). True with
 * *rx_ns uses that time; false, no hook, or a time after socket_ns or more
 * than 100 ms before it keeps socket_ns, as without a hook. */
typedef bool (*apple_clock_rx_time_hook_t)(uint8_t type, uint16_t seq,
                                           const uint8_t port_id[10],
                                           int64_t socket_ns, int64_t *rx_ns);
void apple_clock_set_rx_time_hook(apple_clock_rx_time_hook_t hook);
