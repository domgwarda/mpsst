#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="build"

TEST_TREE_DIR="$SCRIPT_DIR/../../../test_data/test4/tree"
mkdir -p "$TEST_TREE_DIR"


echo "[*] Building project..."
cmake -S . -B "$BUILD_DIR" -DNO_OUTPUT=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$BUILD_DIR" 

chmod +x "$SCRIPT_DIR"/test4_hs.sh && chmod +x "$SCRIPT_DIR"/test4_pcre.sh

for i in {1..100}; do
    if (( i % 20 == 0 )); then
        TEST_TREE_DIR="$SCRIPT_DIR/../../../test_data/test4/tree/branch$i"
        mkdir -p "$TEST_TREE_DIR"
    fi

    base64 /dev/urandom | head -c 10MB > "$TEST_TREE_DIR/file$i.txt"
done

TEST_TREE_DIR="$SCRIPT_DIR/../../../test_data/test4/tree"

"$SCRIPT_DIR"/test4_hs.sh
"$SCRIPT_DIR"/test4_pcre.sh



if [[ "$TEST_TREE_DIR" == *"/test_data/test4/tree" ]]; then
    rm -rf "$TEST_TREE_DIR"
else
    echo "Refusing to delete: $TEST_TREE_DIR"
    exit 1
fi
