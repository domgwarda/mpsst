#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

prog="$SCRIPT_DIR/../../build/main"
indir="$SCRIPT_DIR/infiles/f_test1"
resdir="$SCRIPT_DIR/results"
expdir="$SCRIPT_DIR/expOut"
bin_regex="$SCRIPT_DIR/regex/regexTest.bin"
cpu=10
climit=1000000

ofile="${resdir}/r_test1_bin"
efile="${expdir}/eo_test2.txt"
dfile=$(mktemp)

mkdir -p "$resdir"

ulimit -t ${cpu}
${prog} -f "${indir}" -b "${bin_regex}" -e "hs" &> /dev/null
retv=$?

if [ $retv -ne 0 ]; then
    echo "test 2 failed: non-zero exit status"
    exit $retv
else
    ulimit -t ${cpu}
    ${prog} -f "${indir}" -b "${bin_regex}" -e "hs"  2>&1 | head -c ${climit} \
        | sed "s|$SCRIPT_DIR/|test/|" > "${ofile}"

    sort -o "${ofile}" "${ofile}"
    sort -o "${efile}" "${efile}"

    if diff "${ofile}" "${efile}" &> "${dfile}"; then
        echo "test 2 passed"
        rm "${dfile}"
        exit 0
    else
        echo "test 2 failed"
        cat "${dfile}"
        rm "${dfile}"
        exit 1
    fi
fi
