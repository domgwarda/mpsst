#!/bin/bash

prog="./build/main"
indir="test/infiles/f_test1"
resdir="test/results"
expdir="test/expOut"
regex="test/regex/regexTest.rgx"
cpu=10
climit=1000000

ofile="${resdir}/r_test1"
efile="${expdir}/eo_test1.txt"
dfile=$(mktemp)

mkdir -p "$resdir"

ulimit -t ${cpu}
${prog} -f "${indir}" -r "${regex}" &> /dev/null
retv=$?

if [ $retv -ne 0 ]; then
    echo "f_test1 failed: non-zero exit status"
    exit $retv
else
    ulimit -t ${cpu}
    ${prog} -f "${indir}" -r "${regex}" 2>&1 | head -c ${climit} > "${ofile}"

    if diff "${ofile}" "${efile}" &> "${dfile}"; then
        echo "f_test1 passed"
        rm "$dfile"
        exit 0
    else
        echo "f_test1 failed"
        cat "${dfile}"
        rm "$dfile"
        exit 1
    fi
fi
