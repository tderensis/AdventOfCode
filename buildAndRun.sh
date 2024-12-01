#!/bin/bash

mkdir -p build
cd build/
cmake ..
cmake --build .
cd ../

find ./build/2024/day* -maxdepth 0 -exec bash -c './{}/solution {}/input.txt;' \;
