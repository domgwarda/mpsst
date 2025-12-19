#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../build/main"
indir="$SCRIPT_DIR/infiles"
regex="$SCRIPT_DIR/regex/regexes.rgx"

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
