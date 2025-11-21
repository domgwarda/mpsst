#!/bin/sh
# Test checks if RegexHandler correctly loads all regex patterns from a file.

set -e
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

BIN_DIR=$(pwd)   

echo -e "aaa\naba\ncc" > "$TMPDIR/test.rgx"

cat > "$TMPDIR/test_load.cpp" << EOF
#include <iostream>
#include <string>
#include "$BIN_DIR/regex_handler.h"

int main() {
    RegexHandler rh("$TMPDIR/test.rgx");
    rh.load_regex_file();
    std::cout << rh.size << std::endl;
    return 0;
}
EOF

g++ -std=c++20 "$TMPDIR/test_load.cpp" "$BIN_DIR/regex_handler.cpp" -I"$BIN_DIR" -lhs -o "$TMPDIR/test_load"

OUTPUT=$("$TMPDIR/test_load")
RET=$?

RET=$?
if [ $RET -ne 0 ]; then
    echo "TEST 3 FAIL: Test program returned non-zero exit code $RET"
    exit 1
fi

if [ "$OUTPUT" -ne 3 ]; then
    echo "TEST 3 FAIL: expected 3 regexes, got $OUTPUT"
    exit 1
fi

echo "TEST 3 PASS"
