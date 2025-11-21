#!/bin/sh
# Test checks if main program correctly handles missing command line arguments (regex and file).

TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

TMPREGEX=$(mktemp)
trap "rm -f $TMPREGEX" EXIT

BIN=./build/main

echo "aaa" > "$TMPREGEX"

FAIL=0
TOTAL=4

$BIN >/dev/null 2>&1
RET=$?
if [ $RET -ne 2 ]; then
    echo "FAIL: No arguments (expected 2, got $RET)"
    FAIL=$((FAIL + 1))
fi

$BIN --regex "$TMPREGEX" >/dev/null 2>&1
RET=$?
if [ $RET -ne 3 ]; then
    echo "FAIL: Only regex (expected 3, got $RET)"
    FAIL=$((FAIL + 1))
fi

$BIN --file "$TMPDIR" >/dev/null 2>&1
RET=$?
if [ $RET -ne 2 ]; then
    echo "FAIL: Only file (expected 2, got $RET)"
    FAIL=$((FAIL + 1))
fi

$BIN --regex "$TMPREGEX" --file "$TMPDIR" >/dev/null 2>&1
RET=$?
if [ $RET -ne 0 ]; then
    echo "FAIL: Regex and file (expected 0, got $RET)"
    FAIL=$((FAIL + 1))
fi


PASSED=$((TOTAL - FAIL))

if [ $FAIL -eq 0 ]; then
    echo "TEST 2 PASS"
else
    echo "Test summary: $PASSED / $TOTAL passed, $FAIL / $TOTAL failed"
    echo "TEST 2 FAIL"
fi
