#!/bin/bash

run_test() {
    local DAY=$1
    local TEST=$2

    if [ "${TEST}" = "test" ]; then
        INPUT=test_input.txt
        EXPECTED=test_input_expected.txt
    else
        INPUT=input.txt
        EXPECTED=input_expected.txt
    fi

    RESULT="$(./build/2024/day${DAY}/day${DAY} 2024/day${DAY}/${INPUT})" || return 1

    EXPECTED_STRING="$(cat 2024/day${DAY}/${EXPECTED})"

    if [ "${RESULT}" == "${EXPECTED_STRING}" ]; then
        RESULT_STRING="\033[32mPASS\033[0m" # Green
    else
        RESULT_STRING="\033[31mFAIL\033[0m" # Red
    fi

    echo -e "Day ${DAY} : ${RESULT_STRING}"
    echo "Input: ${INPUT}"
    echo "${RESULT}"
    echo "Expected"
    echo "${EXPECTED_STRING}"
}

DAY=${1}
TEST=${2}

if [ -z "${DAY}" ]; then
    # No day specified, run all days
    for d in $(ls -d 2024/day*/ | cut -d'/' -f2 | cut -d'y' -f2 | sort -n); do
        run_test ${d} ${TEST}
        echo "----------------------------------------"
    done
else
    run_test ${DAY} ${TEST}
fi