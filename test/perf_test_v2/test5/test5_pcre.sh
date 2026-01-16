#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../../build/main"

regex_dir="test_data/test5/regex"
indir="test_data/test5/tree"

# regex_counts=(10 20 50 100)
regex_files=(100 500 750 1000 2500)

for rcount in "${regex_files[@]}"; do
    regex_file="${regex_dir}/regex_${rcount}.rgx"
    echo "Times for: PCRE test with $rcount regexes"
    ${prog} -f "${indir}" -r "${regex_file}" -e pcre -t -w 1 -h 1
    ${prog} -f "${indir}" -r "${regex_file}" -e pcre -t -w 1 -h 5
    ${prog} -f "${indir}" -r "${regex_file}" -e pcre -t -w 1 -h 10
    ${prog} -f "${indir}" -r "${regex_file}" -e pcre -t -w 1 -h 20
done
