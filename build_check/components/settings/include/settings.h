#pragma once
/* Stub of the firmware's settings component: the part ap2_core uses. */
#include <stddef.h>
#include "esp_err.h"

esp_err_t settings_get_device_name(char *name, size_t len);
esp_err_t settings_set_device_name(const char *name);
esp_err_t settings_get_volume(float *volume_db);
esp_err_t settings_set_volume(float volume_db);
esp_err_t settings_persist_volume(void);
