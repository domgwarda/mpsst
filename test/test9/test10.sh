#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# project root directory = two levels above SCRIPT_DIR
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"
OUTPUT_FILE="test/test9/res10.txt"
BUILD_DIR="build"
EXEC_NAME="main"


# paths to test resources
REGEX_FILE="test/unit_test/regex/t10.rgx"
ROOT_DIR="test/t10"

# > "$REGEX_FILE"
# for i in $(seq -w 1 9999); do
#     echo "foo$i.*" >> "$REGEX_FILE"
# done

# OUT="test/t10/text.txt"
# > "$OUT"
# for i in $(seq -w 1000 1999); do
#     printf "foo%04d%s\n" "$i" "$(printf '%*s' 50 | tr ' ' 'x')" >> "$OUT"
# done


# if the executable does not exist – configure and build it
if [[ ! -x "$BUILD_DIR/$EXEC_NAME" ]]; then
    echo "[*] Building $EXEC_NAME in $BUILD_DIR..."
    cmake -S . -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR" --target "$EXEC_NAME"
fi


"./$BUILD_DIR/$EXEC_NAME" -r "$REGEX_FILE" -f "$ROOT_DIR" -w 0 -t 1 -e pcre >> "$OUTPUT_FILE"