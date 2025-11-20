#!/usr/bin/env bash
set -e
# Checks whether the program prints a message when required arguments are missing (--regex)

BIN=build/main
if [ ! -x "$BIN" ]; then
    echo "02_no_args: FAIL (binary not found, run 01_build.sh first)"
    exit 2
fi

OUTPUT=$("$BIN" 2>&1 || true)

if echo "$OUTPUT" | grep -q "Regex path is missing"; then
    echo "02_no_args: PASS"
    exit 0
else
    echo "02_no_args: FAIL"
    exit 1
fi
