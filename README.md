# esp-airplay2-core

The part of an ESP32 AirPlay 2 receiver that talks to Apple, as two ESP-IDF
components:

| Component | What it does |
|---|---|
| [`ap2_core`](components/ap2_core) | HAP pairing and HomeKit, the RTSP server, the mDNS announcement, plist, the FairPlay setup replies, audio decryption |
| [`apple_clock`](components/apple_clock) | the PTP clock towards Apple: owns UDP 319/320 and follows the sender's time with two-way Sync/Delay_Req exchanges |

Both are protocol only. They use sockets, cryptography, a timer and other
ESP-IDF APIs, and nothing about the chip or the board: no
`CONFIG_IDF_TARGET_*`, no pins, no Wi-Fi driver calls. Everything else
belongs to the firmware that uses them: the audio engine and decoders, the
web pages, the display, the network set-up and the board.

Used by:
- [airplay-esp32](https://github.com/Kristian8606/airplay-esp32): a single
  AirPlay 2 speaker;
- [esp32-airplay2-surround](https://github.com/Kristian8606/esp32-airplay2-surround):
  a Gateway that relays AirPlay audio to surround speakers. The speakers
  link `ap2_core` only for audio decryption (`ap2_audio_crypto.h`); the
  linker takes nothing else from it, so they need no more of the contract
  below than the link step.

## Using it

Add both components to the firmware's `main/idf_component.yml`, pinned to a
tag:

```yaml
dependencies:
  ap2_core:
    git: https://github.com/Kristian8606/esp-airplay2-core.git
    path: components/ap2_core
    version: v1.0.0
  apple_clock:
    git: https://github.com/Kristian8606/esp-airplay2-core.git
    path: components/apple_clock
    version: v1.0.0
```

A change here reaches a firmware only when that firmware moves to the new
tag, after its own test.

### Working on the components

While developing, point the firmware at a local checkout and try as often as
needed, without tags:

```yaml
  ap2_core:
    git: https://github.com/Kristian8606/esp-airplay2-core.git
    path: components/ap2_core
    version: v1.0.0
    override_path: ../../esp-airplay2-core/components/ap2_core
```

When it works: commit and tag here, remove `override_path` and raise the
version in the firmware. `override_path` is never committed in a firmware.

### Versions

- `x.y.Z`: a fix; firmwares use the components the same way. Usually needed
  by both firmwares, each moving to it after its own test.
- `x.Y.0`: something new; existing firmwares keep working without changes.
- `X.0.0`: firmwares must be changed.

Different firmwares on different versions is temporary (until the test), not
permanent.

Something only one firmware needs must not change the behaviour of the
other. It comes as a hook: a `ap2_port_*` function each firmware implements
(empty if unused), or a Kconfig option that is off by default. The
components only pass the data on; the firmware decides what to do with it.

## The contract: what the firmware provides

`ap2_core` calls the firmware. Everything in this section must exist, with
these names, in every firmware that uses it.

### 1. Components by name

`ap2_core` lists these in `PRIV_REQUIRES`, so the firmware must have
components with exactly these names:

| Component | Header | What `ap2_core` uses |
|---|---|---|
| `helpers` | `rtsp_events.h` | `rtsp_events_emit()`, `RTSP_EVENT_CLIENT_CONNECTED`, `_PLAYING`, `_PAUSED`, `_DISCONNECTED` |
| `wifi` | `wifi.h` | `wifi_set_hostname()`, `wifi_sanitize_hostname()`, `wifi_get_mac_str()`, `WIFI_HOSTNAME_MAX_LEN` |
| `settings` | `settings.h` | `settings_get_device_name()`, `settings_set_device_name()`, `settings_get_volume()`, `settings_set_volume()`, `settings_persist_volume()` |

`settings.h` may differ between firmwares (the surround firmware adds LFE
and speaker settings). `ap2_core` uses only the device name and the volume,
and these five functions must be the same everywhere.

### 2. Headers of the firmware's main component

These live in the firmware itself; `ap2_core` reaches them through the link
step below.

**`sound_engine.h`**, the audio engine:
- types `sound_format_t`, `sound_cipher_t`, `sound_stream_t`;
- constants `SOUND_CIPHER_NONE`, `SOUND_CIPHER_CHACHA20_POLY1305`,
  `SOUND_STREAM_NONE`, `SOUND_STREAM_REALTIME`, `SOUND_STREAM_BUFFERED`;
- `AP2_BUFFERED_AUDIO_ADVERTISED_BYTES`: the buffered audio capacity told
  to the sender in SETUP (6 MiB in both firmwares);
- `sound_engine_set_format()`, `_set_encryption()`, `_set_stream_type()`,
  `_start_stream()`, `_start_buffered()`, `_get_buffered_port()`, `_stop()`,
  `_set_playing()`, `_pause()`, `_set_anchor_time()`,
  `_set_client_control()`, `_set_immediate_flush()`,
  `_set_deferred_flush_range()`, `_realtime_flush_to_rtp()`,
  `_set_playout_latency_samples()`, `_set_volume_q15()`.

They may be functions or `static inline` wrappers (airplay-esp32 maps them
onto its `audio_receiver_*`).

**`amp_control.h`**: `amp_control_session_connected()`,
`amp_control_session_disconnected()`.

**`airplay_identity.h`**, how the receiver presents itself:
- `AIRPLAY_MODEL`, `AIRPLAY_SOURCE_VERSION`, `AIRPLAY_SERVER_HEADER`,
  `AIRPLAY_PROTOVERS`, `AIRPLAY_PROTOCOL_VERSION`;
- `AIRPLAY_FEATURES_HI`, `AIRPLAY_FEATURES_LO`;
- `AIRPLAY_SUPPORTED_AUDIO_STREAM`, `AIRPLAY_SUPPORTED_BUFFER_STREAM` and
  the `AIRPLAY_FMT_*` bits they are made of;
- `AIRPLAY_PAIRING_ID_LEN`, `AIRPLAY_SERIAL_LEN`;
- `airplay_get_pairing_id()`, `airplay_get_serial_number()`,
  `airplay_get_firmware_revision()`, `airplay_status_flags()`,
  `airplay_set_session_active()`.

`apple_clock.h` is not part of the contract: `ap2_core` requires the
`apple_clock` component of this repository and calls its
`apple_clock_select()` and `apple_clock_release()`.

### 3. The link step

The firmware gives `ap2_core` the include directories of its main component
and links it with that component, after `idf_component_register()` in
`main/CMakeLists.txt`:

```cmake
idf_component_get_property(ap2_core_lib ap2_core COMPONENT_LIB)
target_include_directories(${ap2_core_lib} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/audio"
    "${CMAKE_CURRENT_SOURCE_DIR}/network")
target_link_libraries(${ap2_core_lib} PRIVATE ${COMPONENT_LIB})
```

esp32-airplay2-surround does this in `airplay_link_ap2_core()` in
`common/common.cmake`; airplay-esp32 in a loop at the end of
`main/CMakeLists.txt`.

### 4. Hook functions (`ap2_port.h`)

| Function | When `ap2_core` calls it | What the firmware does |
|---|---|---|
| `void ap2_port_homekit_reset_schedule(void)` | with HomeKit, after acknowledging an authenticated removal from Apple Home | hands the HomeKit reset (erase the keys, reboot) to its own task and returns at once, without heap allocation |
| `esp_err_t ap2_port_device_mac(uint8_t mac[6])` | at start-up (HomeKit identity) and for `/info` (`deviceid`) | gives the MAC that Apple sees on the network, the same on every boot |

On the ESP32-S3 both firmwares implement `ap2_port_device_mac()` with
`esp_read_mac(mac, ESP_MAC_WIFI_STA)`.

### 5. Kconfig options

The firmware defines these (in both firmwares they are in
`components/airplay_options/Kconfig.projbuild`):

| Option | Type | Use |
|---|---|---|
| `AIRPLAY_HOMEKIT` | bool | pairing with Apple Home, `/configure`, HomeKit reset |
| `AIRPLAY_HOMEKIT_MANUFACTURER` | string | the manufacturer Home shows |
| `AIRPLAY_HOMEKIT_SETUP_CODE` | string | the setup code for adding to Home |
| `ENABLE_AIRPLAY_ARTWORK` | bool | announce artwork support (`md=0,1,2`) |
| `AIRPLAY_LOG_RTSP_REQUESTS` | bool | log every RTSP request, pair-verify timing and volume changes |

`apple_clock` defines its own `AIRPLAY_APPLE_CLOCK_ASYM_US` (the path
asymmetry correction, default 0); the firmware does not.

### 6. Managed components

`ap2_core` brings `espressif/libsodium` (`^1.0.21`) and `espressif/mdns`
(`^1.14.1`) in its own `idf_component.yml`. A firmware that also lists
them must use versions within these ranges.

## What the firmware calls

Start-up, in this order, after settings and Wi-Fi:

1. `apple_clock_start()`
2. `ap2_hap_identity_init()`
3. `mdns_airplay_init()`
4. with HomeKit: `ap2_pairings_set_changed_cb()`
5. `rtsp_server_start()`

Later:
- `rtsp_server_stop()`, `rtsp_server_is_idle()`: stop AirPlay (Wi-Fi scan,
  OTA, calibration) and start it again with `rtsp_server_start()`;
- `rtsp_server_begin_reset()`: before stopping for a reset; nothing can
  start the server again until the reboot;
- `rtsp_remote_enqueue()`, `rtsp_remote_get_status()`,
  `rtsp_remote_result_name()`: play, pause and skip commands to the sender
  (buttons, web page);
- `ap2_audio_decrypt_rtp()`, `ap2_audio_decrypt_buffered()`: from the audio
  engine;
- `apple_clock_get()`, `apple_clock_at()`, `apple_clock_to_local()`: the
  sender's time for playout;
- `ap2_pairings_has_admin()`: whether the receiver is in a home.

The headers in `components/*/include` describe every function.

## Build check

[`build_check/`](build_check) builds both components with stubs of
everything in the contract: the three components, the three headers, the
link step, the hooks and the Kconfig options. It is compiled and linked,
never run. If a change here needs something new from the firmware, this
build fails until the stubs, and this README, say so.

It is built for `esp32s3`, with HomeKit on (with artwork and the RTSP log)
and off, on every push
([`.github/workflows/build.yml`](.github/workflows/build.yml), ESP-IDF
v6.0.2). Locally, with ESP-IDF exported:

```sh
tools/build_check.sh
```

## Documentation

- [`docs/rtsp-spec.md`](docs/rtsp-spec.md): behaviour of the RTSP server;
- [`docs/mdns-airplay-spec.md`](docs/mdns-airplay-spec.md): behaviour of the
  mDNS announcement;
- [`components/ap2_core/README.md`](components/ap2_core/README.md): layout
  and the FairPlay setup.

## License

Apache License 2.0 ([`LICENSE`](LICENSE), [`NOTICE`](NOTICE)). Each
component carries its own copy of the license and a NOTICE. Not covered:
`components/ap2_core/src/fairplay/fairplay_tables.c`, fixed interop data
(see [`components/ap2_core/NOTICE`](components/ap2_core/NOTICE)), and the
components fetched by the ESP-IDF component manager, each under its own
license.
