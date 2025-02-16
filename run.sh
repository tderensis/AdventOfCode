#!/bin/bash

DAY=${1}
TEST=${2}

if [ "${TEST}" = "test" ]; then
  echo "Test Input"
  ./build/2024/day${DAY}/day${DAY} 2024/day${DAY}/test_input.txt || exit 1
else
  echo "Real Input"
  ./build/2024/day${DAY}/day${DAY} 2024/day${DAY}/input.txt
fi
