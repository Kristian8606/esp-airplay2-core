#pragma once
/* Stub of the firmware's helpers component: the part ap2_core uses. */
typedef enum {
  RTSP_EVENT_CLIENT_CONNECTED,
  RTSP_EVENT_PLAYING,
  RTSP_EVENT_PAUSED,
  RTSP_EVENT_DISCONNECTED,
} rtsp_event_t;

void rtsp_events_emit(rtsp_event_t event);
