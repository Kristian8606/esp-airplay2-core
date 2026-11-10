#include "wifi.h"
#include <stdio.h>
void wifi_get_mac_str(char *mac_str, size_t len) {
  if (len) snprintf(mac_str, len, "00:00:00:00:00:00");
}
void wifi_sanitize_hostname(const char *name, char *out, size_t out_len) {
  if (out_len) snprintf(out, out_len, "%s", name ? name : "");
}
void wifi_set_hostname(const char *device_name) { (void)device_name; }
