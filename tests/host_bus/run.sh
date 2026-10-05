#!/bin/sh
set -eu
test_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(CDPATH= cd -- "$test_dir/../.." && pwd)
test_binary=$(mktemp "${TMPDIR:-/tmp}/inverter-bus-trace.XXXXXX")
trap 'rm -f "$test_binary"' EXIT HUP INT TERM
"${CXX:-c++}" -std=c++11 -Wall -Wextra -Werror -pedantic \
    -I"$test_dir/stubs" -I"$repo_dir/src" \
    "$test_dir/test_bus_trace.cpp" "$repo_dir/src/InverterModbusBus.cpp" \
    -o "$test_binary"
"$test_binary"
