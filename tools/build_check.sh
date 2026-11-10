#!/usr/bin/env bash
# Builds build_check/ for every chip and configuration, as the CI does.
# Run from anywhere, with ESP-IDF exported (. $IDF_PATH/export.sh).
# Arguments limit the chips: tools/build_check.sh esp32s3
set -euo pipefail
cd "$(dirname "$0")/../build_check"

targets=("$@")
[ ${#targets[@]} -eq 0 ] && targets=(esp32s3 esp32p4)
configs=(homekit plain)

for target in "${targets[@]}"; do
  for config in "${configs[@]}"; do
    dir="build_${target}_${config}"
    echo "== ${target} / ${config}"
    rm -rf "$dir" # set-target refuses to clean an unfinished build directory
    idf.py -B "$dir" -D SDKCONFIG="$dir/sdkconfig" \
      -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.ci.${config}" \
      set-target "$target" build
  done
done
echo "== all builds passed"
