#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="build"

TEST_TREE_DIR="$SCRIPT_DIR/../../../perf_test_data/test6/tree"
mkdir -p "$TEST_TREE_DIR"


echo "[*] Building project..."
cmake -S . -B "$BUILD_DIR" -DNO_OUTPUT=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$BUILD_DIR" 

chmod +x "$SCRIPT_DIR"/test6_hs.sh && chmod +x "$SCRIPT_DIR"/test6_pcre.sh

for i in {1..70}; do
    if (( i % 20 == 0 )); then
        TEST_TREE_DIR="$SCRIPT_DIR/../../../perf_test_data/test6/tree/branch$i"
        mkdir -p "$TEST_TREE_DIR"
    fi

    base64 /dev/urandom | head -c 3MB > "$TEST_TREE_DIR/file$i.txt"
done

TEST_TREE_DIR="$SCRIPT_DIR/../../../perf_test_data/test6/tree"

{
  echo "70,"
  "$SCRIPT_DIR"/test6_pcre.sh
} >> "$SCRIPT_DIR"/res_pcre.txt



if [[ "$TEST_TREE_DIR" == *"/perf_test_data/test6/tree" ]]; then
    rm -rf "$TEST_TREE_DIR"
else
    echo "Refusing to delete: $TEST_TREE_DIR"
    exit 1
fi
