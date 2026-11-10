idf_component_register(
    SRCS "main.c" "stubs.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES ap2_core apple_clock helpers wifi settings
)

# The link step of the contract: ap2_core gets this component's headers
# (sound_engine.h, amp_control.h, airplay_identity.h) and links with it.
idf_component_get_property(ap2_core_lib ap2_core COMPONENT_LIB)
target_include_directories(${ap2_core_lib} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${ap2_core_lib} PRIVATE ${COMPONENT_LIB})
