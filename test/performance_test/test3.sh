#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# project root directory = two levels above SCRIPT_DIR
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="build"
EXEC_NAME="main"

# paths to test resources
REGEX_FILE="test_data/regexes/regex_sensitive_data.rgx"
ROOT_DIR="test_data/test_tree_big"

# if the executable does not exist – configure and build it
if [[ ! -x "$BUILD_DIR/$EXEC_NAME" ]]; then
    echo "[*] Building $EXEC_NAME in $BUILD_DIR..."
    cmake -S . -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR"
fi

"./$BUILD_DIR/$EXEC_NAME" -r "$REGEX_FILE" -f "$ROOT_DIR" -t -h 6 -e pcre
