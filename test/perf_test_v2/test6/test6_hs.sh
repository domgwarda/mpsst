#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../../build/main"

regex="perf_test_data/test6/regex.rgx"
indir="perf_test_data/test6/tree"

${prog} -f "${indir}" -r "${regex}" -e hs -t 3 -w 1 -h 100
