# Build check: ap2_core and apple_clock with stubs of everything the
# firmware provides (README.md, "The contract"). It builds and links for
# every chip; it is not meant to run.
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../components")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ap2_build_check)
