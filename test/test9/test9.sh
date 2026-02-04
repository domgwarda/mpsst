#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# project root directory = two levels above SCRIPT_DIR
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"
OUTPUT_FILE="test/test9/result.txt"
BUILD_DIR="build"
EXEC_NAME="main"


# paths to test resources
REGEX_FILE="test/unit_test/regex/t9.rgx"
ROOT_DIR="test/t9"

# if the executable does not exist – configure and build it
if [[ ! -x "$BUILD_DIR/$EXEC_NAME" ]]; then
    echo "[*] Building $EXEC_NAME in $BUILD_DIR..."
    cmake -S . -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR" --target "$EXEC_NAME"
fi


"./$BUILD_DIR/$EXEC_NAME" -r "$REGEX_FILE" -f "$ROOT_DIR" -w 5 -t 10 -e hs >> "$OUTPUT_FILE"