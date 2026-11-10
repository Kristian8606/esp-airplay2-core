#include "settings.h"
#include <stdio.h>
esp_err_t settings_get_device_name(char *name, size_t len) {
  if (len) snprintf(name, len, "Build check");
  return ESP_OK;
}
esp_err_t settings_set_device_name(const char *name) { (void)name; return ESP_OK; }
esp_err_t settings_get_volume(float *volume_db) { *volume_db = -20.0f; return ESP_OK; }
esp_err_t settings_set_volume(float volume_db) { (void)volume_db; return ESP_OK; }
esp_err_t settings_persist_volume(void) { return ESP_OK; }
