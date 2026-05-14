#!/bin/bash
set -e

BACKEND=${1:-x11}
GENERATOR=${2:-}  # optional: "Ninja", "Unix Makefiles", etc.

GENERATOR_ARGS=()
if [ -n "$GENERATOR" ]; then
    GENERATOR_ARGS=(-G "$GENERATOR")
elif command -v ninja &>/dev/null; then
    GENERATOR_ARGS=(-G Ninja)
fi

cmake -B build/cmake \
      -DCMAKE_BUILD_TYPE=Debug \
      -DRAZOR_GLFW_BACKEND="$BACKEND" \
      "${GENERATOR_ARGS[@]}"

echo ""
echo "CMake project generated at build/cmake/"
echo "Build with: cmake --build build/cmake"
