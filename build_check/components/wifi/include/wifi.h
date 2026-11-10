#pragma once
/* Stub of the firmware's wifi component: the part ap2_core uses. */
#include <stddef.h>

#define WIFI_HOSTNAME_MAX_LEN 31

void wifi_get_mac_str(char *mac_str, size_t len);
void wifi_sanitize_hostname(const char *name, char *out, size_t out_len);
void wifi_set_hostname(const char *device_name);
