#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
cd "$PROJECT_ROOT"

BUILD_DIR="build"
TEST_TREE_DIR="$SCRIPT_DIR/../../../test_data/test5/tree"
REGEX_DIR="$SCRIPT_DIR/../../../test_data/test5/regex"

mkdir -p "$TEST_TREE_DIR"
mkdir -p "$REGEX_DIR"

echo "[*] Building project..."
cmake -S . -B "$BUILD_DIR" -DNO_OUTPUT=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build "$BUILD_DIR" 

chmod +x "$SCRIPT_DIR"/test5_hs.sh "$SCRIPT_DIR"/test5_pcre.sh

for i in {1..10}; do
    file_path="$TEST_TREE_DIR/file${i}.txt"
    echo "[*] Generating 10MB file: $file_path"
    base64 /dev/urandom | head -c 10MB > "$file_path"
done


regex_files=(100 500 750 1000)
for count in "${regex_counts[@]}"; do
    regex_file="$REGEX_DIR/regex_${count}.rgx"
    > "$regex_file"
    for ((i=1;i<=count;i++)); do
        length=$((RANDOM % 11 + 10)) # 10–20 znaków
        rand_str=$(base64 /dev/urandom | tr -dc 'a-z' | head -c "$length")
        echo "$rand_str" >> "$regex_file"
    done
done

"$SCRIPT_DIR"/test5_hs.sh
"$SCRIPT_DIR"/test5_pcre.sh


if [[ "$TEST_TREE_DIR" == *"/test_data/test5/tree" ]]; then
    rm -rf "$TEST_TREE_DIR"
    rm -rf "$REGEX_DIR"
else
    echo "Refusing to delete: $TEST_TREE_DIR"
    exit 1
fi
