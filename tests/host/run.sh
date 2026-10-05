#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
test_dir="$(mktemp -d)"
trap 'rm -rf "$test_dir"' EXIT
"${CXX:-g++}" -std=c++17 -O1 -g -ffunction-sections -fdata-sections \
  -fsanitize=undefined,float-cast-overflow -fno-sanitize-recover=all \
  -I"$repo_dir/tests/host/stubs" -I"$repo_dir/src" \
  "$repo_dir/tests/host/test_async_power_limit.cpp" \
  "$repo_dir/src/InverterControl.cpp" "$repo_dir/src/InverterDeviceInfo.cpp" \
  "$repo_dir/src/InverterModbusBus.cpp" "$repo_dir/src/InverterInternalHelpers.cpp" \
  "$repo_dir/src/ModbusConfig.cpp" "$repo_dir/src/InverterMaps_Weg.cpp" \
  "$repo_dir/src/InverterDescriptors_Weg.cpp" \
  -Wl,--gc-sections -o "$test_dir/test_async_power_limit"
"$test_dir/test_async_power_limit"
