#!/bin/sh
set -e
# Test checks if the project builds correctly.

BUILD_DIR=build
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"


if cmake -S . -B "$BUILD_DIR" >/dev/null 2>&1 && cmake --build "$BUILD_DIR" >/dev/null 2>&1; then
  echo "TEST 1 PASS"
  exit 0
else
  echo "TEST 1: FAIL"
  exit 1
fi
