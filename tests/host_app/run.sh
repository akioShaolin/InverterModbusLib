#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
test_dir="$(mktemp -d)"
trap 'rm -rf "$test_dir"' EXIT
"${CXX:-g++}" -std=c++17 -O1 -g -ffunction-sections -fdata-sections \
  -fsanitize=undefined,float-cast-overflow -fno-sanitize-recover=all \
  -I"$repo_dir/tests/host_app/stubs" -I"$repo_dir/tests/host/stubs" -I"$repo_dir/src" \
  "$repo_dir/tests/host_app/test.cpp" "$repo_dir/src/FieldTestLog.cpp" \
  "$repo_dir/src/InverterControl.cpp" "$repo_dir/src/InverterDeviceInfo.cpp" \
  "$repo_dir/src/InverterModbusBus.cpp" "$repo_dir/src/InverterInternalHelpers.cpp" \
  "$repo_dir/src/ModbusConfig.cpp" "$repo_dir/src/InverterMaps_Weg.cpp" \
  "$repo_dir/src/InverterDescriptors_Weg.cpp" \
  -Wl,--gc-sections -o "$test_dir/test_app"
"$test_dir/test_app" > "$test_dir/output.txt"
python3 - "$test_dir/output.txt" <<'PY'
import csv, io, json, pathlib, sys
text = pathlib.Path(sys.argv[1]).read_text()
status = json.loads(text.split('STATUS_JSON=', 1)[1].splitlines()[0])
assert status['paused'] and not status['stopping'] and status['command']['status'] == 'DONE'
rows = list(csv.reader(io.StringIO(text.split('CSV_BEGIN\n', 1)[1].split('CSV_END\n', 1)[0])))
assert all(len(row) == 20 for row in rows), [len(row) for row in rows]
assert rows[1][-1] == 'host,"case"'
assert int(status['log']['bytes']) > 0
print(text.splitlines()[-1])
print(f'PASS: parsed real firmware status JSON and {len(rows)} CSV rows, each with 20 columns.')
PY
