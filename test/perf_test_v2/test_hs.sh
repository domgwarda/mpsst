#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../build/main"

indir="$1"
regex="$2"

start_hs=$(date +%s%3N)   
${prog} -f "${indir}" -r "${regex}" -e hs >/dev/null 2>&1
end_hs=$(date +%s%3N)
hs_diff=$((end_hs - start_hs))

echo "HS   Time difference = ${hs_diff}[ms]"
