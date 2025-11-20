#!/usr/bin/env bash
set -e
# Checks whether the program prints a message when --file argument is missing

BIN=build/main
if [ ! -x "$BIN" ]; then
    echo "03_only_regex: FAIL (binary not found, run 01_build.sh first)"
    exit 2
fi

# Run program with only --regex argument, capture all output
OUTPUT=$("$BIN" --regex regexTest.rgx 2>&1 || true)

# Always print the program output for visibility
echo "$OUTPUT"

# Check for expected message
if echo "$OUTPUT" | grep -q "Root path is missing"; then
    echo "03_only_regex: PASS"
    exit 0
else
    echo "03_only_regex: FAIL (expected message 'Root path is missing' not found)"
    exit 1
fi
