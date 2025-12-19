#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../build/main"

indir="$1"
regex="$2"

hs_time=$(
  ${prog} -f "${indir}" -r "${regex}" -e hs -t 2>/dev/null \
  | grep "Time difference"
)

pcre_time=$(
  ${prog} -f "${indir}" -r "${regex}" -e pcre -t 2>/dev/null \
  | grep "Time difference"
)

echo "HS   ${hs_time}"
echo "PCRE ${pcre_time}"
