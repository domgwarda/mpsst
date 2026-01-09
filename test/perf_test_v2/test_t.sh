#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../build/main"

indir="$1"
regex="$2"

hs_time=$(
  ${prog} -f "${indir}" -r "${regex}" -e hs -w 5 -t 100>/dev/null \
  | grep "avg="
)

pcre_time=$(
  ${prog} -f "${indir}" -r "${regex}" -e pcre -w 5 -t 100>/dev/null \
  | grep "avg="
)

echo "HS   ${hs_time}"
echo "PCRE ${pcre_time}"
