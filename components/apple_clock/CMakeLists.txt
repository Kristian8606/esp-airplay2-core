# apple_clock: the PTP clock towards Apple (Apache-2.0). Owns UDP 319/320
# and estimates the sender's time from two-way Sync/Delay_Req exchanges.
# Uses only ESP-IDF; apple_clock_core.c is the pure math.
idf_component_register(
    SRCS
        "src/apple_clock.c"
        "src/apple_clock_core.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    PRIV_REQUIRES esp_timer esp_hw_support lwip
)
