#!/usr/bin/env bash
set -e
# Checks if the project builds correctly using CMake and C++20, linking Hyperscan and Boost

BUILD_DIR=build
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"


if cmake -S . -B "$BUILD_DIR" >/dev/null 2>&1 && cmake --build "$BUILD_DIR" >/dev/null 2>&1; then
  echo "01_build: PASS"
  exit 0
else
  echo "01_build: FAIL"
  exit 1
fi
