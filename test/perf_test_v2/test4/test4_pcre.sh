#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../../build/main"

regex="test_data/test4/regex_sensitive.rgx"
indir="test_data/test4/tree"

${prog} -f "${indir}" -r "${regex}" -e pcre -t -w 1 -h 1
${prog} -f "${indir}" -r "${regex}" -e pcre -t -w 1 -h 5
${prog} -f "${indir}" -r "${regex}" -e pcre -t -w 1 -h 10
${prog} -f "${indir}" -r "${regex}" -e pcre -t -w 1 -h 20

