#!/bin/bash

set -e

EXECUTABLE="./bin/Debug-linux-x86_64/Edge/Edge"

perf record \
    -g \
    --call-graph fp \
    -o perf.data \
    "$EXECUTABLE"

echo "==> Running perf script..."

perf script -i perf.data > out.perf &
PERF_PID=$!

while kill -0 "$PERF_PID" 2>/dev/null; do
    LINES=$(wc -l < out.perf 2>/dev/null || echo 0)
    SIZE=$(du -h out.perf 2>/dev/null | cut -f1 || echo "0")
    printf "\r    Still processing... %s lines, %s" "$LINES" "$SIZE"
    sleep 2
done

wait "$PERF_PID"

echo
echo "==> perf script finished"

./scripts/stackcollapse-perf.pl out.perf > out.folded
./scripts/flamegraph.pl out.folded > flamegraph.svg

code flamegraph.svg
