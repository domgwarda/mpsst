#!/bin/bash

testdir="test"
passed=0
failed=0

for tfile in "$testdir"/*.sh; do
    if [ "$(basename "$tfile")" != "all_tests.sh" ]; then
        echo "Running $tfile..."
        bash "$tfile"
        ret=$?
        if [ $ret -eq 0 ]; then
            passed=$((passed+1))
        else
            failed=$((failed+1))
        fi
    fi
done

echo
echo "Test summary: $passed passed, $failed failed"

if [ $failed -eq 0 ]; then
    exit 0
else
    exit 1
fi
